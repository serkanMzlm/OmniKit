#include "bus_interface.hpp"
#include "udp.hpp"

#include <thread>
#include <cmath>
#include <algorithm>
#include <system_error>

Udp::Udp(const std::string &listen_ip, int listen_port, int target_port)
{
    //  _opt.listen
    _options.listen_ip = listen_ip;
    _options.target_ip = listen_ip;
    _options.target_port = static_cast<uint16_t>(std::clamp(target_port, 1, 65535));
    _options.listen_port = static_cast<uint16_t>(std::clamp(listen_port, 1, 65535));
    _timestamp = std::chrono::steady_clock::now();
}

Udp::Udp(const UdpOptions &opts) : _options(opts)
{
    _timestamp = std::chrono::steady_clock::now();
}

Udp::~Udp()
{
    disconnect();
}

bool Udp::isValidPort(int port)
{
    return port > 0 && port <= 65535;
}

int Udp::waitReadable(int timeout_ms)
{
    _pollfds[0].events = POLLIN;
    _pollfds[0].revents = 0;

    while (true)
    {
        int rv = ::poll(_pollfds, 1, timeout_ms);
        if (rv < 0 && errno == EINTR)
            continue;
        return rv; // -1 hata, 0 timeout, >0 hazır
    }
}

bool Udp::connect()
{
    std::lock_guard<std::mutex> lock(_mtx);
    _timestamp = std::chrono::steady_clock::now();

    int backoff_ms = 50;
    while (!init())
    {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - _timestamp);

        if (elapsed.count() >= getTimeout())
        {
            setOpenPort(false);
            disconnect();
            std::cerr << "Connection timeout" << std::endl;
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

bool Udp::init()
{
    if (_fd >= 0)
        return true;

    if (!isValidPort(_options.listen_port) || !isValidPort(_options.target_port))
    {
        std::cerr << "Invalid port(s)\n";
        return false;
    }

    _fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (_fd < 0)
    {
        std::cerr << "Failed to create socket: " << strerror(errno) << std::endl;
        return false;
    }

    try
    {
        if (!setupSocketOptions())
        {
            return false;
        }
        if (!bindSocket())
        {
            return false;
        }

        setupPoll();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Initialization failed: " << e.what() << std::endl;
        disconnect();
        return false;
    }

    return true;
}

bool Udp::setupSocketOptions()
{
    if (fcntl(_fd, F_SETFL, O_NONBLOCK) < 0)
    {
        std::cerr << "fcntl(O_NONBLOCK) failed: " << strerror(errno) << std::endl;
        return 0;
    }

    int enable = 1;
    if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) < 0)
    {
        std::cerr << "setsockopt(SO_REUSEADDR) failed: " << strerror(errno) << std::endl;
        return 0;
    }

    if (setsockopt(_fd, SOL_SOCKET, SO_BROADCAST, &enable, sizeof(enable)) < 0)
    {
        std::cerr << "setsockopt(SO_BROADCAST) failed: " << strerror(errno) << std::endl;
        return 0;
    }
    return 1;
}

bool Udp::bindSocket()
{
    memset(&_listen_addr, 0, sizeof(_listen_addr));
    _listen_addr.sin_family = AF_INET;

    if (_options.listen_ip == "0.0.0.0" || _options.listen_ip.empty())
    {
        _listen_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    else
    {
        _listen_addr.sin_addr.s_addr = inet_addr(_options.listen_ip.c_str());
        if (_listen_addr.sin_addr.s_addr == INADDR_NONE)
        {
            std::cerr << "Invalid local IP address: " << _options.listen_ip << " | " << strerror(errno) << std::endl;
            return false;
        }
    }

    _listen_addr.sin_port = htons(_options.listen_port);

    if (bind(_fd, reinterpret_cast<struct sockaddr *>(&_listen_addr), sizeof(_listen_addr)) < 0)
    {
        std::cerr << "Listen IP bind failed on : " << _options.listen_ip << ":" << std::to_string(_options.listen_port) << " | " << strerror(errno) << std::endl;
        return false;
    }

    memset(&_target_addr, 0, sizeof(_target_addr));
    _target_addr.sin_family = AF_INET;

    if (_options.target_ip == "0.0.0.0" || _options.target_ip.empty())
    {
        _target_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    else
    {
        _target_addr.sin_addr.s_addr = inet_addr(_options.target_ip.c_str());
        if (_target_addr.sin_addr.s_addr == INADDR_NONE)
        {
            std::cerr << "Invalid Target IP address: " << _options.target_ip << " | " << strerror(errno) << std::endl;
            return false;
        }
    }

    _target_addr.sin_port = htons(_options.target_port);

    return true;
}

void Udp::setupPoll()
{
    _pollfds[0].fd = _fd;
    _pollfds[0].events = POLLIN;
}

void Udp::disconnect()
{
    if (_fd >= 0)
    {
        close(_fd);
        _fd = -1;
    }

    setOpenPort(false);
}

int Udp::read(uint8_t *buffer, size_t size)
{
    std::lock_guard<std::mutex> lock(_mtx);
    if (!isOpenPort() || _fd < 0)
    {
        std::cerr << "Port not open" << std::endl;
        return -1;
    }

    int retval = poll(_pollfds, 1, 100);
    if (retval < 0)
    {
        if (errno == EINTR)
        {
            return -1;
        }
        std::cerr << "Poll failed:" << strerror(errno) << std::endl;
        return -1;
    }
    if (retval == 0)
        return 0;

    if (_pollfds[0].revents & POLLIN)
    {
        socklen_t addr_len = sizeof(_listen_addr);

        int read_bytes = recvfrom(_fd, buffer, size, 0,
                                  reinterpret_cast<struct sockaddr *>(&_listen_addr), &addr_len);

        if (read_bytes < 0)
        {
#if EAGAIN != EWOULDBLOCK
            if (errno == EAGAIN || errno == EWOULDBLOCK)
#else
            if (errno == EAGAIN)
#endif
            {
                return 0;
            }
            std::cerr << "Recvfrom failed:" << strerror(errno) << std::endl;
            return -1;
        }

        return read_bytes;
    }

    if (_pollfds[0].revents & (POLLERR | POLLHUP | POLLNVAL))
    {
        std::cerr << "Socket error detected" << std::endl;
        return -1;
    }

    return 0;
}

int Udp::write(const uint8_t *buffer, size_t size)
{
    std::lock_guard<std::mutex> lock(_mtx);
    if (!isOpenPort() || _fd < 0)
    {
        std::cerr << "Port not open" << std::endl;
        return -1;
    }

    int written = sendto(_fd, buffer, size, 0,
                         reinterpret_cast<struct sockaddr *>(&_target_addr), sizeof(_target_addr));

    if (written < 0)
    {
        std::cerr << "Write failed: " << strerror(errno) << std::endl;
        return -1;
    }
    return written;
}

int Udp::writeByte(const uint8_t *byte)
{
    return this->write(byte, 1);
}

int Udp::readByte(uint8_t *byte)
{
    return this->read(byte, 1);
}

void Udp::getConnectionInfo() const
{
    std::cout << "[UDP] Listen: " + _options.listen_ip + ":" + std::to_string(_options.listen_port) +
                     " -> Target: " + _options.target_ip + ":" + std::to_string(_options.target_port)
              << std::endl;
}

void Udp::setTargetAddress(const std::string &ip, int port)
{
    std::lock_guard<std::mutex> lock(_mtx);

    if (!isValidIpAddress(ip))
    {
        return;
    }

    _options.target_ip = ip;
    _options.target_port = port;

    memset(&_target_addr, 0, sizeof(_target_addr));
    _target_addr.sin_family = AF_INET;
    _target_addr.sin_addr.s_addr = inet_addr(_options.target_ip.c_str());
    _target_addr.sin_port = htons(_options.target_port);
}

void Udp::setListenAddress(const std::string &ip, int port)
{
    std::lock_guard<std::mutex> lock(_mtx);

    if (!isValidIpAddress(ip))
    {
        return;
    }

    _options.listen_ip = ip;
    _options.listen_port = port;

    memset(&_listen_addr, 0, sizeof(_listen_addr));
    _listen_addr.sin_family = AF_INET;
    _listen_addr.sin_addr.s_addr = inet_addr(_options.listen_ip.c_str());
    _listen_addr.sin_port = htons(_options.listen_port);
}

std::string Udp::getTargetAddress() const
{
    std::lock_guard<std::mutex> lock(_mtx);
    return _options.target_ip + ":" + std::to_string(_options.target_port);
}

std::string Udp::getListenAddress() const
{
    std::lock_guard<std::mutex> lock(_mtx);
    return _options.listen_ip + ":" + std::to_string(_options.listen_port);
}

bool Udp::isValidIpAddress(const std::string &ip) const
{
    struct sockaddr_in sa;
    return inet_pton(AF_INET, ip.c_str(), &(sa.sin_addr)) != 0;
}

bool Udp::setConfig(const UdpOptions &opts)
{
    std::lock_guard<std::mutex> lk(_mtx);
    if (isOpenPort())
    {
        std::cerr << "Cannot change config while port is open" << std::endl;
        return false;
    }
    _options = opts;
    return true;
}

UdpOptions Udp::getConfig() const
{
    std::lock_guard<std::mutex> lk(_mtx);
    return _options;
}