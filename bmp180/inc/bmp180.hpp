#ifndef __BMP180_HPP__
#define __BMP180_HPP__

#include <iostream>
#include <cstring>
#include <cmath>
#include "bmp180_type.hpp"

class BMP180{
public:
    BMP180(int bus_number = 1);
    ~BMP180();
    int initI2c(const char * filename, int bmp_addr);
    void readCalibrationCoef();
    void prinfCalbrationCoef() const;
    void reportError(int error, std::string error_info = "errno") const;

private:
    int fd;
    char filename[11];
    int16_t AC1, AC2, AC3;
    uint16_t AC4, AC5, AC6;
    int16_t B1, B2;
    int16_t MB, MC, MD;
};

#endif