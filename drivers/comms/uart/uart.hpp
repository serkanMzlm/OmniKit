#ifndef UART_HPP
#define UART_HPP

#include <termios.h>
#include <poll.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <cerrno>
#include <cstdint>
#include <cstring>
#include <mutex>
#include <string>
#include <chrono>
#include <iostream>


class BusInterface;

enum class Parity : uint8_t
{
    None,
    Even,
    Odd
};
enum class StopBits : uint8_t
{
    One,
    Two
};
enum class FlowControl : uint8_t
{
    None,
    RTSCTS,
    XOnXOff
};

struct UartOptions
{
    std::string device;
    speed_t baud{115200};
    uint8_t data_bits{8};
    Parity parity{Parity::None};
    StopBits stop_bits{StopBits::One};
    FlowControl flow_control{FlowControl::None};
};

class Uart : public BusInterface
{
public:
    Uart(std::string dev, speed_t baudrate = B115200) noexcept;
    explicit Uart(const UartOptions& opts) noexcept;
    ~Uart();
    
    Uart(const Uart &) = delete;
    Uart &operator=(const Uart &) = delete;

    bool init() override;
    bool connect() override;
    void disconnect() override;

    int writeByte(const uint8_t *byte) override;
    int write(const uint8_t *buffer, size_t size) override;

    int readByte(uint8_t *byte) override;
    int read(uint8_t *buffer, size_t size) override;

    void getConnectionInfo() const override;
    std::string baudrateToString(speed_t baud) const;
    bool setConfig(const UartOptions& opts);
    UartOptions getConfig() const;

private:
    bool configure();
    bool setExclusive();
    int waitReadable(int timeout_ms);

    static speed_t mapBaud(unsigned int bps, bool& ok);
    static bool    applyDataBits(termios& tio, uint8_t bits);
    static void    applyParity(termios& tio, Parity p);
    static void    applyStopBits(termios& tio, StopBits s);
    static void    applyFlow(termios& tio, FlowControl f);

private:
    UartOptions _options;

    char buf[8];
    struct pollfd _pollfd{ -1, POLLIN, 0 };

    mutable std::mutex _mtx;
    std::chrono::steady_clock::time_point _timestamp;
};

#endif 