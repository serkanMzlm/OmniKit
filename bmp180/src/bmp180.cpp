#include "bmp180.hpp"

BMP180::BMP180(int bus_number) {
    sprintf(filename, "/dev/i2c-%d", bus_number);

    if (initI2c(filename, 0x77) < 0) {
        exit(1);
    }

    readCalibrationData();
}

BMP180::~BMP180() {
    close(fd);
}

long BMP180::readRawTemperature() const {
    int data = i2c_smbus_read_word_data(fd, 0xF6);
    return (data << 8) | ((data & 0xFF00) >> 8);
}

long BMP180::readRawPressure() const {
    i2c_smbus_write_byte_data(fd, 0xF4, 0x34 + (OSS << 6));
    usleep(30000); // Bekleme süresi OSS (Over Sampling Setting) değerine bağlı olarak değişebilir

    int msb = i2c_smbus_read_byte_data(fd, 0xF6);
    int lsb = i2c_smbus_read_byte_data(fd, 0xF7);
    int xlsb = i2c_smbus_read_byte_data(fd, 0xF8);

    long rawPressure = ((msb << 16) | (lsb << 8) | xlsb) >> (8 - OSS);

    return rawPressure;
}


short BMP180::readShort(int address) const {
    int data = i2c_smbus_read_word_data(fd, address);
    return (data << 8) | ((data & 0xFF00) >> 8);
}

unsigned short BMP180::readUShort(int address) const {
    int data = i2c_smbus_read_word_data(fd, address);
    return (data << 8) | ((data & 0xFF00) >> 8);
}

void BMP180::readCalibrationData() {
    ac1 = readShort(0xAA);
    ac2 = readShort(0xAC);
    ac3 = readShort(0xAE);
    ac4 = readUShort(0xB0);
    ac5 = readUShort(0xB2);
    ac6 = readUShort(0xB4);
    b1 = readShort(0xB6);
    b2 = readShort(0xB8);
    mb = readShort(0xBA);
    mc = readShort(0xBC);
    md = readShort(0xBE);
}

int BMP180::initI2c(const char* filename, int bmp_addr) {
    fd = open(filename, O_RDWR);
    if (fd < 0) {
        perror("Could not open the I2C device");
        return -1;
    }

    if (ioctl(fd, I2C_SLAVE, bmp_addr) < 0) {
        perror("Could not set I2C device address");
        close(fd);
        return -1;
    }

    return 1;
}

void BMP180::reportError(int error) {
    std::cerr << "Error! Errno: " << strerror(error);
}

double BMP180::getTemperature() const {
    long UT = readRawTemperature();
    long X1 = (UT - ac6) * ac5 >> 15;
    long X2 = (mc << 11) / (X1 + md);
    long B5 = X1 + X2;
    long T = (B5 + 8) >> 4;
    return static_cast<double>(T) / 10.0;
}

double BMP180::getPressure() const {
    long UT = readRawTemperature();
    long UP = readRawPressure();
    long X1 = (UT - ac6) * ac5 >> 15;
    long X2 = (mc << 11) / (X1 + md);
    long B5 = X1 + X2;
    long B6 = B5 - 4000;
    long X3 = ((b2 * (B6 * B6 >> 12)) >> 11) + (ac2 * B6 >> 11);
    long X4 = (ac1 * ((b6 * b6 >> 12)) >> 13) + (b1 * B6 >> 16) + 2;
    long X5 = (X1 * X1 >> 13) + (X2 * B6 >> 16);
    long B3 = ((X3 + X4 + X5) + 3) >> 2;
    long B4 = (ac4 * (unsigned long)(B3 + 32768) >> 15);
    long B7 = ((unsigned long)UP - B4) * (50000 >> OSS);

    long P;
    if (B7 < 0x80000000) {
        P = (B7 << 1) / B3;
    } else {
        P = (B7 / B3) << 1;
    }

    X1 = (P >> 8) * (P >> 8);
    X1 = (X1 * 3038) >> 16;
    X2 = (-7357 * P) >> 16;

    return P + ((X1 + X2 + 3791) >> 4);
}

int main() {
    BMP180 bmp180(1);

    std::cout << "Temperature: " << std::fixed << std::setprecision(2) << bmp180.getTemperature() << " °C\n";
    std::cout << "Pressure: " << std::fixed << std::setprecision(2) << bmp180.getPressure() << " Pa\n";

    return 0;
}