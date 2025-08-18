#include "bus_interface.hpp"
#include "uart.hpp"

Uart::Uart(std::string dev, speed_t baudrate) noexcept
{
    _options.baud = baudrate;
    _options.device = dev;
    _options.data_bits = 8;
    _options.flow_control = FlowControl::None;
    _options.parity = Parity::None;
    _options.stop_bits = StopBits::One;
}

Uart::Uart(const UartOptions &opts) noexcept : _options(opts)
{
}

Uart::~Uart()
{
    try
    {
        disconnect();
    }
    catch (...)
    {
    }
}

bool Uart::setExclusive()
{
#ifdef TIOCEXCL
    if (::ioctl(_fd, TIOCEXCL) == -1)
    {
        return false;
    }
#endif
    return true;
}

bool Uart::init()
{
    std::lock_guard<std::mutex> lk(_mtx);
    _fd = open(_options.device.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    // _fd = open(_options.device.c_str(), O_RDWR | O_NOCTTY | O_CLOEXEC);

    if (_fd < 0)
    {
        std::cerr << "Uart Port: " << _options.device << " couldn't be opened:" << strerror(errno) << std::endl;
        return false;
    }

    fcntl(_fd, F_SETFL, 0);

    if (!setExclusive())
    {
        disconnect();
        return false;
    }

    if (!configure())
    {
        disconnect();
        std::cerr << "Uart Port Couldn't Configure!" << std::endl;
        return false;
    }

    _pollfd.fd = _fd;
    _pollfd.events = POLLIN;
    _pollfd.revents = 0;
    return true;
}

bool Uart::configure()
{
    termios tio{};

    if (tcgetattr(_fd, &tio) != 0)
    {
        std::cerr << "Tcgetattr failed: " << std::string(strerror(errno)) << std::endl;
        return false;
    }

    cfmakeraw(&tio);

    applyParity(tio, _options.parity);
    applyStopBits(tio, _options.stop_bits);
    applyFlow(tio, _options.flow_control);

    const uint32_t ms = getTimeout();
    const uint8_t vtime = static_cast<uint8_t>((ms + 99) / 100);
    tio.c_cc[VTIME] = vtime;
    tio.c_cc[VMIN] = 0;

    if (::cfsetispeed(&tio, _options.baud) != 0)
    {
        return false;
    }
    if (::cfsetospeed(&tio, _options.baud) != 0)
    {
        return false;
    }

    tcflush(_fd, TCIOFLUSH);

    if (::tcsetattr(_fd, TCSANOW, &tio) != 0)
    {
        std::cerr << "Error from tcsetattr: " << std::string(strerror(errno));
        return false;
    }

    return true;
}

bool Uart::connect()
{
    _timestamp = std::chrono::steady_clock::now();

    int backoff_ms = 50;
    while (true)
    {
        if (init())
        {
            setOpenPort(true);
            getConnectionInfo();
            return true;
        }

        auto now = std::chrono::steady_clock::now();
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - _timestamp).count();

        if (elapsed_ms > getTimeout())
        {
            std::cerr << "Connection Timeout!" << std::endl;
            setOpenPort(false);
            return false;
        }
        usleep(static_cast<useconds_t>(backoff_ms * 1000));
        if (backoff_ms < 500)
            backoff_ms *= 2;
    }

    setOpenPort(true);
    getConnectionInfo();
    return true;
}

void Uart::disconnect()
{
    std::lock_guard<std::mutex> lk(_mtx);
    if (_fd >= 0)
    {
        tcdrain(_fd);
        close(_fd);
        _fd = -1;
    }
    setOpenPort(false);
    _pollfd.fd = -1;
    _pollfd.revents = 0;
}

int Uart::waitReadable(int timeout_ms)
{
    _pollfd.events = POLLIN;
    _pollfd.revents = 0;
    while (true)
    {
        int rv = ::poll(&_pollfd, 1, timeout_ms);
        if (rv < 0)
        {
            if (errno == EINTR)
                continue;
            return -1;
        }
        return rv;
    }
}

int Uart::read(uint8_t *buffer, size_t size)
{
    if (!buffer || size == 0)
    {
        return 0;
    }

    std::lock_guard<std::mutex> lk(_mtx);
    if (!isOpenPort() || _fd < 0)
    {
        return -1;
    }

    int rv = waitReadable(static_cast<int>(getTimeout()));
    if (rv <= 0)
        return rv; // -1 error, 0 timeout

    size_t total = 0;
    while (total < size)
    {
        ssize_t n = ::read(_fd, buffer + total, size - total);
        if (n > 0)
        {
            total += static_cast<size_t>(n);
            break;
        }
        else if (n == 0)
        {
            return static_cast<int>(total);
        }
        else
        {
            if (errno == EINTR)
            {
                continue;
            }

#if EAGAIN != EWOULDBLOCK
            if (errno == EAGAIN || errno == EWOULDBLOCK)
#else
            if (errno == EAGAIN)
#endif
            {
                break;
            }
            break;
            return (total > 0) ? static_cast<int>(total) : -1;
        }
    }

    return static_cast<int>(total);
}

int Uart::write(const uint8_t *buffer, size_t size)
{
    if (!buffer || size == 0)
    {
        return 0;
    }

    std::lock_guard<std::mutex> lock(_mtx);
    if (!isOpenPort() || _fd < 0)
    {
        return -1;
    }

    size_t total = 0;
    const auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(getTimeout());

    while (total < size)
    {
        ssize_t n = ::write(_fd, buffer + total, size - total);
        if (n > 0)
        {
            total += static_cast<size_t>(n);
            continue;
        }
        if (n < 0 && errno == EINTR)
        {
            continue;
        }

#if EAGAIN != EWOULDBLOCK
        if (errno == EAGAIN || errno == EWOULDBLOCK)
#else
        if (errno == EAGAIN)
#endif
        {
            int left_ms = static_cast<int>(std::max<int64_t>(
                0, std::chrono::duration_cast<std::chrono::milliseconds>(deadline - std::chrono::steady_clock::now()).count()));
            if (left_ms == 0)
                break;
            struct pollfd wfd{_fd, POLLOUT, 0};
            int rv = ::poll(&wfd, 1, left_ms);
            if (rv <= 0)
                break; // timeout veya error
            continue;
        }
        return (total > 0) ? static_cast<int>(total) : -1;
    }
    return static_cast<int>(total);
}

int Uart::writeByte(const uint8_t *byte)
{
    return this->write(byte, 1);
}

int Uart::readByte(uint8_t *byte)
{
    return this->read(byte, 1);
}

void Uart::getConnectionInfo() const
{
    if (!isOpenPort())
    {
        std::cout << "Uart Could Not Connect (" +
                         _options.device + ":" + baudrateToString(_options.baud) + ")\n";
    }
    else
    {
        std::cout << "Uart is Successfully Connected: " +
                         _options.device + ":" + baudrateToString(_options.baud) + ")\n";
    }
}

bool Uart::setConfig(const UartOptions &opts)
{
    std::lock_guard<std::mutex> lk(_mtx);
    _options = opts;

    if (_fd < 0)
    {
        return true;
    }

    return configure();
}

UartOptions Uart::getConfig() const
{
    std::lock_guard<std::mutex> lk(_mtx);
    return _options;
}

bool Uart::applyDataBits(termios &tio, uint8_t bits)
{
    tio.c_cflag &= ~CSIZE;
    switch (bits)
    {
    case 5:
        tio.c_cflag |= CS5;
        return true;
    case 6:
        tio.c_cflag |= CS6;
        return true;
    case 7:
        tio.c_cflag |= CS7;
        return true;
    case 8:
        tio.c_cflag |= CS8;
        return true;
    default:
        return false;
    }
}

void Uart::applyParity(termios &tio, Parity p)
{
    switch (p)
    {
    case Parity::None:
        tio.c_cflag &= ~PARENB;
        break;
    case Parity::Even:
        tio.c_cflag |= PARENB;
        tio.c_cflag &= ~PARODD;
        break;
    case Parity::Odd:
        tio.c_cflag |= PARENB;
        tio.c_cflag |= PARODD;
        break;
    }
}

void Uart::applyStopBits(termios &tio, StopBits s)
{
    if (s == StopBits::Two)
    {
        tio.c_cflag |= CSTOPB;
    }
    else
    {
        tio.c_cflag &= ~CSTOPB;
    }
}

void Uart::applyFlow(termios &tio, FlowControl f)
{
    if (f == FlowControl::RTSCTS)
    {
        tio.c_cflag |= CRTSCTS;
    }
    else
    {
        tio.c_cflag &= ~CRTSCTS;
    }

    if (f == FlowControl::XOnXOff)
    {
        tio.c_iflag |= (IXON | IXOFF | IXANY);
    }
    else
    {
        tio.c_iflag &= ~(IXON | IXOFF | IXANY);
    }
}

speed_t Uart::mapBaud(unsigned int bps, bool &ok)
{
    ok = true;
    switch (bps)
    {
    case 0:
        return B0;
    case 50:
        return B50;
    case 75:
        return B75;
    case 110:
        return B110;
    case 134:
        return B134;
    case 150:
        return B150;
    case 200:
        return B200;
    case 300:
        return B300;
    case 600:
        return B600;
    case 1200:
        return B1200;
    case 1800:
        return B1800;
    case 2400:
        return B2400;
    case 4800:
        return B4800;
    case 9600:
        return B9600;
    case 19200:
        return B19200;
    case 38400:
        return B38400;
    case 57600:
        return B57600;
    case 115200:
        return B115200;
#ifdef B230400
    case 230400:
        return B230400;
#endif
#ifdef B460800
    case 460800:
        return B460800;
#endif
#ifdef B500000
    case 500000:
        return B500000;
#endif
#ifdef B576000
    case 576000:
        return B576000;
#endif
#ifdef B921600
    case 921600:
        return B921600;
#endif
    default:
        ok = false;
        return 0;
    }
}

std::string Uart::baudrateToString(speed_t baud) const
{
    switch (baud)
    {
    case B0:
        return "0";
    case B50:
        return "50";
    case B75:
        return "75";
    case B110:
        return "110";
    case B134:
        return "134";
    case B150:
        return "150";
    case B200:
        return "200";
    case B300:
        return "300";
    case B600:
        return "600";
    case B1200:
        return "1200";
    case B1800:
        return "1800";
    case B2400:
        return "2400";
    case B4800:
        return "4800";
    case B9600:
        return "9600";
    case B19200:
        return "19200";
    case B38400:
        return "38400";
    case B57600:
        return "57600";
    case B115200:
        return "115200";
#ifdef B230400
    case B230400:
        return "230400";
#endif
#ifdef B460800
    case B460800:
        return "460800";
#endif
#ifdef B500000
    case B500000:
        return "500000";
#endif
#ifdef B576000
    case B576000:
        return "576000";
#endif
#ifdef B921600
    case B921600:
        return "921600";
#endif
    default:
        return "Unknown";
    }
}
