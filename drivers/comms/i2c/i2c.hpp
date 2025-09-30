#ifndef I2C_HPP
#define I2C_HPP

#include <mutex>
#include <cerrno>
#include <iostream>

extern "C" {
#include <errno.h>
#include <fcntl.h>
#include <i2c/smbus.h>
#include <linux/i2c-dev.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
}

class BusInterface;

struct I2COptions {
        int       bus_number{};        // /dev/i2c-1
        uint16_t  address{};           // 7-bit adres (10-bit için 0..1023)
        bool      ten_bit{false};      // 10-bit addressing
        bool      enable_pec{false};   // SMBus PEC
        int       retries{3};          // I2C_RETRIES (adapter destekliyorsa)
        int       timeout_ms{1000};    // I2C_TIMEOUT (adapter destekliyorsa, 10ms birim)
};

class I2C : public BusInterface
{
public:
    explicit I2C(int bus_number);
    explicit I2C(const I2COptions& opts);

    ~I2C();

    bool init() override;
    bool connect() override;
    void disconnect() override;

    int write(const uint8_t *data, size_t size) override;
    int read(uint8_t *buffer, size_t size) override;
    int readByte(uint8_t *byte) override;
    int writeByte(const uint8_t *byte) override;
    void getConnectionInfo() const override;

    bool setAddress(uint16_t address, bool ten_bit = false);
    bool writeRegByte(uint8_t reg, uint8_t val);
    bool readRegByte(uint8_t reg);
    ssize_t writeReg(uint8_t reg, const uint8_t* data, size_t len);
    ssize_t readReg(uint8_t reg, uint8_t* data, size_t len);

    bool setConfig(const I2COptions& opt);
    I2COptions getConfig() const;

private:
    bool applyAdapterOptions(); // retries/timeout/pec/10bit
    bool ensureAddressSelected(); // her işlem öncesi doğru slave seçili mi

private:
    I2COptions _options;
    char _filename[11]; // "/dev/i2c-X"
    mutable std::mutex _mtx;
};

#endif // I2C_HPP