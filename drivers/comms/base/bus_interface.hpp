#ifndef BUS_INTERFACE_HPP
#define BUS_INTERFACE_HPP

#include <cstdio>
#include <string>
#include <cstdint>

#include "comm_types.hpp"

class BusInterface {
public: 
    virtual ~BusInterface() = default;

    BusInterface(const BusInterface&) = delete;
    BusInterface& operator=(const BusInterface&) = delete;

    virtual bool init() = 0;
    virtual bool send(const uint8_t* data, size_t length) = 0;
    virtual bool receive(uint8_t* buffer, size_t length) = 0;

    virtual CommType getCommType() const = 0;

    CommType getType() const { return _type; }
    uint32_t getTimeout() const { return _timeout_ms; }
    bool isOpenPort() const { return _is_open; }
    
    void setTimeout(uint32_t timeout);
    void setType(CommType type);
    void setOpenPort(bool is_open);

protected:
    int _fd{-1};
    const int delay_us {500000};

private:
    CommType _type{CommType::UNKNOWN};
    uint32_t _timeout_ms{1000};
    bool _is_open{false};

    static constexpr uint32_t MIN_TIMEOUT_MS = 100;
    static constexpr uint32_t MAX_TIMEOUT_MS = 5000;
};

#endif // BUS_INTERFACE_HPP