#ifndef SPI_HPP
#define SPI_HPP

#include <cstdint>
#include <string>
#include <mutex>
#include <iostream>

extern "C" {
#include <linux/spi/spidev.h>
}

struct SpiOptions {
        int      bus{0};            // /dev/spidev<bus>.<cs>
        int      cs{0};
        uint32_t maxSpeedHz{1'000'000}; // 1 MHz
        uint8_t  mode{0};           // 0..3 (CPOL/CPHA)
        uint8_t  bitsPerWord{8};    // 8, 16...
        bool     lsbFirst{false};
        bool     csHigh{false};
        bool     threeWire{false};  // SPI_3WIRE
        bool     loopback{false};   // SPI_LOOP
        bool     noCs{false};       // SPI_NO_CS
        uint16_t delayUsec{0};      // transfer gecikmesi
        std::string devPath{};      // boşsa /dev/spidev<bus>.<cs> türetilecek
};

class Spi : public BusInterface {
public:
    Spi(int bus, int cs, uint32_t speed_hz = 1'000'000, uint8_t mode = 0, uint8_t bits = 8);
    explicit Spi(const SpiOptions& opt);
    ~Spi() override;

    bool init() override;
    bool connect() override;
    void disconnect() override;

    int  write(const uint8_t* data, size_t size) override; 
    int  read(uint8_t* buffer, size_t size) override;      
    int  readByte(uint8_t* byte) override;
    int  writeByte(const uint8_t* byte) override;
    void getConnectionInfo() const override;

    int  transfer(const uint8_t* tx, uint8_t* rx, size_t size);
    int  writeRead(const uint8_t* tx, uint8_t* rx, size_t size) { return transfer(tx, rx, size); }
    bool setConfig(const SpiOptions& opt);
    SpiOptions getConfig() const { std::lock_guard<std::mutex> lk(_mtx); return _opt; }

private:
    bool openDevice();
    bool applyConfig();
    static bool validate(const SpiOptions& opt);
    static std::string pathFrom(const SpiOptions& opt);

private:
    SpiOptions _opt;
    std::string _devPath;
    mutable std::mutex _mtx;
};

#endif // SPI_HPP