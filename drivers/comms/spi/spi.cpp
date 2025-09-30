#include "bus_interface.hpp"
#include "spi.hpp"

extern "C"
{
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
}

#include <cstring>
#include <algorithm>

static uint8_t modeFrom(const SpiOptions &o)
{
    uint8_t m = 0;
    // low 2 bits CPOL/CPHA (mode 0..3)
    m |= (o.mode & 0x3);
#ifdef SPI_CS_HIGH
    if (o.csHigh)
        m |= SPI_CS_HIGH;
#endif
#ifdef SPI_3WIRE
    if (o.threeWire)
        m |= SPI_3WIRE;
#endif
#ifdef SPI_LOOP
    if (o.loopback)
        m |= SPI_LOOP;
#endif
#ifdef SPI_NO_CS
    if (o.noCs)
        m |= SPI_NO_CS;
#endif
    return m;
}

bool Spi::validate(const SpiOptions &o)
{
    if (o.bitsPerWord == 0)
        return false;
    if (o.mode > 3)
        return false;
    if (o.maxSpeedHz == 0)
        return false;
    return true;
}

std::string Spi::pathFrom(const SpiOptions &opt)
{
    if (!opt.devPath.empty())
        return opt.devPath;
    char p[32];
    std::snprintf(p, sizeof(p), "/dev/spidev%d.%d", opt.bus, opt.cs);
    return std::string(p);
}

Spi::Spi(int bus, int cs, uint32_t speed_hz, uint8_t mode, uint8_t bits)
{
    _opt.bus = bus;
    _opt.cs = cs;
    _opt.maxSpeedHz = speed_hz;
    _opt.mode = mode;
    _opt.bitsPerWord = bits;
}

Spi::Spi(const SpiOptions &opt) : _opt(opt) {}

Spi::~Spi()
{
    try
    {
        disconnect();
    }
    catch (...)
    {
    }
}

bool Spi::openDevice()
{
    _devPath = pathFrom(_opt);
    // O_CLOEXEC: alt proseslere fd sızmasın
    _fd = ::open(_devPath.c_str(), O_RDWR | O_CLOEXEC);
    if (_fd < 0)
    {
        std::cerr << "SPI: open(" << _devPath << ") failed: " << std::strerror(errno) << "\n";
        return false;
    }
    return true;
}

bool Spi::applyConfig()
{
    if (!validate(_opt))
    {
        std::cerr << "SPI: invalid options\n";
        return false;
    }

    uint8_t mode = modeFrom(_opt);
    if (::ioctl(_fd, SPI_IOC_WR_MODE, &mode) < 0)
    {
        std::cerr << "SPI: WR_MODE failed: " << std::strerror(errno) << "\n";
        return false;
    }
    if (::ioctl(_fd, SPI_IOC_RD_MODE, &mode) < 0)
    {
        std::cerr << "SPI: RD_MODE failed: " << std::strerror(errno) << "\n";
        return false;
    }

#ifdef SPI_IOC_WR_LSB_FIRST
    uint8_t lsb = _opt.lsbFirst ? 1 : 0;
    if (::ioctl(_fd, SPI_IOC_WR_LSB_FIRST, &lsb) < 0)
    {
        // std::cerr << "SPI: WR_LSB_FIRST unsupported: " << std::strerror(errno) << "\n";
    }
#endif

    if (::ioctl(_fd, SPI_IOC_WR_BITS_PER_WORD, &_opt.bitsPerWord) < 0)
    {
        std::cerr << "SPI: WR_BITS_PER_WORD failed: " << std::strerror(errno) << "\n";
        return false;
    }

    if (::ioctl(_fd, SPI_IOC_WR_MAX_SPEED_HZ, &_opt.maxSpeedHz) < 0)
    {
        std::cerr << "SPI: WR_MAX_SPEED_HZ failed: " << std::strerror(errno) << "\n";
        return false;
    }

    return true;
}

bool Spi::init()
{
    std::lock_guard<std::mutex> lk(_mtx);

    if (_fd >= 0)
        return true;
    if (!openDevice())
        return false;
    if (!applyConfig())
    {
        ::close(_fd);
        _fd = -1;
        return false;
    }
    return true;
}

bool Spi::connect()
{
    bool ok = init();
    setOpenPort(ok);
    if (ok)
        getConnectionInfo();
    return ok;
}

void Spi::disconnect()
{
    std::lock_guard<std::mutex> lk(_mtx);
    if (_fd >= 0)
    {
        ::close(_fd);
        _fd = -1;
    }
    setOpenPort(false);
}

int Spi::transfer(const uint8_t *tx, uint8_t *rx, size_t size)
{
    if (size == 0)
        return 0;
    std::lock_guard<std::mutex> lk(_mtx);
    if (!isOpenPort() || _fd < 0)
        return -1;

    spi_ioc_transfer tr{};
    tr.tx_buf = reinterpret_cast<unsigned long>(tx);
    tr.rx_buf = reinterpret_cast<unsigned long>(rx);
    tr.len = static_cast<uint32_t>(size);
    tr.speed_hz = _opt.maxSpeedHz;
    tr.bits_per_word = _opt.bitsPerWord;
    tr.delay_usecs = _opt.delayUsec;

    int rc = ::ioctl(_fd, SPI_IOC_MESSAGE(1), &tr);
    if (rc < 0)
    {
        if (errno == EINTR)
            return 0;
        std::cerr << "SPI: transfer failed: " << std::strerror(errno) << "\n";
        return -1;
    }
    return static_cast<int>(size);
}

int Spi::write(const uint8_t *data, size_t size)
{
    if (!data || size == 0)
    {
        return 0;
    }

    return transfer(data, nullptr, size);
}

int Spi::read(uint8_t *buffer, size_t size)
{
    if (!buffer || size == 0)
        return 0;

    std::vector<uint8_t> dummy(size, 0xFF);
    return transfer(dummy.data(), buffer, size);
}

int Spi::writeByte(const uint8_t *byte)
{
    if (!byte)
        return 0;
    return write(byte, 1);
}

int Spi::readByte(uint8_t *byte)
{
    if (!byte)
        return 0;
    return read(byte, 1);
}

bool Spi::setConfig(const SpiOptions &opt)
{
    std::lock_guard<std::mutex> lk(_mtx);

    if (!validate(opt))
        return false;
    bool needReopen = false;

    if (opt.devPath != _opt.devPath || opt.bus != _opt.bus || opt.cs != _opt.cs)
    {
        needReopen = true;
    }

    _opt = opt;

    if (_fd < 0)
        return true; 

    if (needReopen)
    {
        ::close(_fd);
        _fd = -1;
        setOpenPort(false);
        return init() && (setOpenPort(true), true);
    }

    return applyConfig();
}

void Spi::getConnectionInfo() const
{
    std::lock_guard<std::mutex> lk(_mtx);
    std::cout << "[SPI] " << (_devPath.empty() ? pathFrom(_opt) : _devPath)
              << " mode=" << int(_opt.mode)
              << " bits=" << int(_opt.bitsPerWord)
              << " speed=" << _opt.maxSpeedHz << "Hz"
              << " lsbFirst=" << (_opt.lsbFirst ? "yes" : "no")
              << " csHigh=" << (_opt.csHigh ? "yes" : "no")
              << " threeWire=" << (_opt.threeWire ? "yes" : "no")
              << " loop=" << (_opt.loopback ? "yes" : "no")
              << " noCs=" << (_opt.noCs ? "yes" : "no")
              << " delay=" << _opt.delayUsec << "us"
              << (isOpenPort() ? " [open]" : " [closed]") << "\n";
}
