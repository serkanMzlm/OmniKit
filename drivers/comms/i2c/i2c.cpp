#include "bus_interface.hpp"
#include "i2c.hpp"

I2C::I2C(int bus_number)
{
    _options.bus_number = bus_number;
}

I2C::I2C(const I2COptions &opts)
{
    _options = opts;
}

I2C::~I2C()
{
    try
    {
        disconnect();
    }
    catch (...)
    {
    }
}

bool I2C::init()
{
    std::lock_guard<std::mutex> lk(_mtx);
    if (_fd >= 0)
    {
        return true;
    }

    snprintf(_filename, 11, "/dev/i2c-%d", _options.bus_number);
    _fd = open(_filename, O_RDWR | O_CLOEXEC);
    if (_fd < 0)
    {
        std::cerr << "I2C Bus: " << _filename << " couldn't be opened: " << strerror(errno) << std::endl;
        return false;
    }

    if (!applyAdapterOptions())
    {
        ::close(_fd);
        _fd = -1;
        return false;
    }

    return true;
}

bool I2C::applyAdapterOptions()
{
    if (_options.retries > 0)
    {
        if (::ioctl(_fd, I2C_RETRIES, _options.retries) < 0)
        {
            std::cerr << "I2C: I2C_RETRIES unsupported: " << strerror(errno) << "\n";
        }
    }
    // I2C_TIMEOUT: 10ms birim (kernel doc). Yuvarlayalım.
    if (_options.timeout_ms > 0)
    {
        int ticks = std::max(1, _options.timeout_ms / 10);
        if (::ioctl(_fd, I2C_TIMEOUT, ticks) < 0)
        {
            // desteklenmeyebilir; fatal değil
        }
    }
    // PEC (SMBus)
#ifdef I2C_PEC
    int pec = _options.enable_pec ? 1 : 0;
    if (::ioctl(_fd, I2C_PEC, pec) < 0)
    {
        // desteklenmeyebilir; fatal değil
    }
#endif
    return true;
}

bool I2C::ensureAddressSelected()
{
    // 10-bit addressing
    if (::ioctl(_fd, I2C_TENBIT, _options.ten_bit ? 1 : 0) < 0)
    {
        if (_options.ten_bit)
        {
            std::cerr << "I2C: 10-bit addressing not supported: " << strerror(errno) << "\n";
            return false;
        }
    }
    if (::ioctl(_fd, I2C_SLAVE, _options.address) < 0)
    {
        std::cerr << "I2C Bus: " << _filename << " couldn't set slave address: 0x"
                  << std::hex << int(_options.address)
                  << std::dec << " error: " << strerror(errno) << std::endl;
        return false;
    }
    return true;
}

bool I2C::connect()
{
    bool ret = init();
    setOpenPort(ret);
    return ret;
}

void I2C::disconnect()
{
    std::lock_guard<std::mutex> lk(_mtx);
    if (_fd >= 0)
    {
        close(_fd);
        _fd = -1;
    }
    setOpenPort(false);
}

int I2C::write(const uint8_t *data, size_t size)
{
    std::cerr << "I2C::write not implemented for register-based writes. Use writeReg instead.\n";
    (void)data;
    (void)size;
    return -1;
}

int I2C::read(uint8_t *buffer, size_t size)
{
    std::cerr << "I2C::read not implemented for register-based read. Use readReg instead.\n";
    (void)buffer;
    (void)size;
    return -1;
}

int I2C::readByte(uint8_t *byte)
{
    std::cerr << "I2C::readByte not implemented for register-based read. Use readRegByte(reg, val) instead.\n";
    (void)byte;
    return -1;
}

int I2C::writeByte(const uint8_t *byte)
{
    std::cerr << "I2C::writeByte not implemented for register-based writes. Use writeRegByte(reg, val) instead.\n";
    (void)byte;
    return -1;
}

bool I2C::setAddress(uint16_t address, bool ten_bit)
{
    std::lock_guard<std::mutex> lk(_mtx);
    _options.address = address;
    _options.ten_bit = ten_bit;

    return ensureAddressSelected();
}

bool I2C::writeRegByte(uint8_t reg, uint8_t val)
{
    std::lock_guard<std::mutex> lk(_mtx);
    if (!isOpenPort() || _fd < 0)
    {
        return false;
    }

    int rc = ::i2c_smbus_write_byte_data(_fd, reg, val);
    return (rc == 0);
}

bool I2C::readRegByte(uint8_t reg)
{
    std::lock_guard<std::mutex> lk(_mtx);
    if (!isOpenPort())
    {
        return false;
    }

    int rc = ::i2c_smbus_read_byte_data(_fd, reg);
    return (rc < 0);
}

ssize_t I2C::writeReg(uint8_t reg, const uint8_t *data, size_t len)
{
    if (!data || len == 0)
    {
        return 0;
    }
    
    std::lock_guard<std::mutex> lk(_mtx);
    if (!isOpenPort())
    {
        return -1;
    }

    int rc = ::i2c_smbus_write_i2c_block_data(_fd, reg, static_cast<uint8_t>(len), data);
    return (rc < 0);
}

ssize_t I2C::readReg(uint8_t reg, uint8_t *data, size_t len)
{
    if (!data || len == 0)
        return 0;
    std::lock_guard<std::mutex> lk(_mtx);
    if (!isOpenPort())
    {
        return -1;
    }

    int rc = ::i2c_smbus_read_i2c_block_data(_fd, reg, static_cast<uint8_t>(len), data);
    return (rc < 0);
}

bool I2C::setConfig(const I2COptions &opt)
{
    std::lock_guard<std::mutex> lk(_mtx);
    _options = opt;

    if (_fd < 0)
        return true;

    return applyAdapterOptions() && ensureAddressSelected();
}

I2COptions I2C::getConfig() const
{
    std::lock_guard<std::mutex> lk(_mtx);
    return _options;
}

void I2C::getConnectionInfo() const
{
    std::lock_guard<std::mutex> lk(_mtx);
    std::cout << "[I2C] " << _filename
              << " addr=0x" << std::hex << int(_options.address) << std::dec
              << (_options.ten_bit ? " (10-bit)" : " (7-bit)")
              << " PEC=" << (_options.enable_pec ? "on" : "off")
              << " retries=" << _options.retries
              << " timeout(ms)=" << _options.timeout_ms
              << (isOpenPort() ? " [open]" : " [closed]") << "\n";
}