#ifndef BMP180_HPP
#define BMP180_HPP

#include <iomanip>
#include <iostream>
#include <cmath>

extern "C" {
#include <errno.h>
#include <fcntl.h>
#include <i2c/smbus.h>
#include <linux/i2c-dev.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
}

class BMP180 {
public:
    BMP180(int bus_number);
    ~BMP180();

    double getTemperature() const;
    double getPressure() const;

private:
    int fd;
    char filename[20];

    short ac1, ac2, ac3;
    unsigned short ac4, ac5, ac6;
    short b1, b2, mb, mc, md;

    long readRawTemperature() const;
    long readRawPressure() const;
    short readShort(int address) const;
    unsigned short readUShort(int address) const;
    void readCalibrationData();
    int initI2c(const char* filename, int bmp_addr);
    void reportError(int error);
};

#endif // BMP180_HPP