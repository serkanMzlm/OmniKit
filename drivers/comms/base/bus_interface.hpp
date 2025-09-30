#ifndef BUS_INTERFACE_HPP
#define BUS_INTERFACE_HPP

#include <cstdio>
#include <string>
#include <cstdint>

class BusInterface
{
public:
    virtual ~BusInterface() = default;

    BusInterface(const BusInterface &) = delete;
    BusInterface &operator=(const BusInterface &) = delete;

    virtual bool init() = 0;
    virtual bool connect() = 0;
    virtual void disconnect() = 0;

    virtual int write(const uint8_t *data, size_t size) = 0;
    virtual int read(uint8_t *buffer, size_t size) = 0;
    virtual int readByte(uint8_t *byte) = 0;
    virtual int writeByte(const uint8_t *byte) = 0;
    virtual void getConnectionInfo() const = 0;

    uint32_t getTimeout() const { return _timeout_ms; }
    bool isOpenPort() const { return _is_open; }

    void setTimeout(uint32_t timeout);
    void setOpenPort(bool is_open);

protected:
    int _fd{-1};
    explicit BusInterface() = default;

private:
    uint32_t _timeout_ms{1000};
    bool _is_open{false};

    static constexpr uint32_t MIN_TIMEOUT_MS = 100;
    static constexpr uint32_t MAX_TIMEOUT_MS = 5000;
};

#endif // BUS_INTERFACE_HPP