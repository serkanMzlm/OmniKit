extern "C"{
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <i2c/smbus.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <unistd.h>
}

#include "bmp180.hpp"

BMP180::BMP180(int bus_number){
    snprintf(filename, 11, "/dev/i2c-%d", bus_number);
    if(initI2c(filename, BMP180_SLAVE_ADDR) < 0){  exit(-1); }

}

BMP180::~BMP180(){
    close(fd);
}

int BMP180::initI2c(const char * filename, int bmp_addr){
    fd = open(filename, O_RDWR);
    if(fd < -1){
        reportError(errno, "Could not open the I2C device");
        return -1;
    }
    if(ioctl(fd, I2C_SLAVE, bmp_addr) < 0){
        reportError(errno, "Could not open the I2C device address");
        close(fd);
        return -1;
    }
    return 1;
}

void BMP180::readCalibrationCoef(){
    int16_t ac1_h = i2c_smbus_read_byte_data(fd, BMP180_AC1_H);
    int16_t ac1_l = i2c_smbus_read_byte_data(fd, BMP180_AC1_H + 1);
    AC1 = ac1_l | ac1_h << 8;

    int16_t ac2_h = i2c_smbus_read_byte_data(fd, BMP180_AC2_H);
    int16_t ac2_l = i2c_smbus_read_byte_data(fd, BMP180_AC2_H + 1);
    AC2 = ac2_l | ac2_h << 8;

    int16_t ac3_h = i2c_smbus_read_byte_data(fd, BMP180_AC3_H);
    int16_t ac3_l = i2c_smbus_read_byte_data(fd, BMP180_AC3_H + 1);
    AC3 = ac3_l | ac3_h << 8;

    int16_t ac4_h = i2c_smbus_read_byte_data(fd, BMP180_AC4_H);
    int16_t ac4_l = i2c_smbus_read_byte_data(fd, BMP180_AC4_H + 1);
    AC4 = static_cast<uint16_t>(ac4_l | ac4_h << 8);

    int16_t ac5_h = i2c_smbus_read_byte_data(fd, BMP180_AC5_H);
    int16_t ac5_l = i2c_smbus_read_byte_data(fd, BMP180_AC5_H + 1);
    AC5 = static_cast<uint16_t>(ac5_l | ac5_h << 8);

    int16_t ac6_h = i2c_smbus_read_byte_data(fd, BMP180_AC6_H);
    int16_t ac6_l = i2c_smbus_read_byte_data(fd, BMP180_AC6_H + 1);
    AC6 = static_cast<uint16_t>(ac6_l | ac6_h << 8);

    int16_t b1_h = i2c_smbus_read_byte_data(fd, BMP180_B1_H);
    int16_t b1_l = i2c_smbus_read_byte_data(fd, BMP180_B1_H + 1);
    B1 = b1_l | b1_h << 8;

    int16_t b2_h = i2c_smbus_read_byte_data(fd, BMP180_B2_H);
    int16_t b2_l = i2c_smbus_read_byte_data(fd, BMP180_B2_H + 1);
    B2 = b2_l | b2_h << 8;

    int16_t mb_h = i2c_smbus_read_byte_data(fd, BMP180_MB_H);
    int16_t mb_l = i2c_smbus_read_byte_data(fd, BMP180_MB_H + 1);
    MB = mb_l | mb_h << 8;

    int16_t mc_h = i2c_smbus_read_byte_data(fd, BMP180_MC_H);
    int16_t mc_l = i2c_smbus_read_byte_data(fd, BMP180_MC_H + 1);
    MC = mc_l | mc_h << 8;

    int16_t md_h = i2c_smbus_read_byte_data(fd, BMP180_MD_H);
    int16_t md_l = i2c_smbus_read_byte_data(fd, BMP180_MD_H + 1);
    MD = md_l | md_h << 8;
}

void BMP180::calculatePressure(){
}

int16_t BMP180::readRawTemperature(){
    int result = i2c_smbus_write_byte_data(fd, BMP180_CONTROL, BMP180_TEMP_CMD);
    if(result < 0) reportError(errno);
    usleep(5*1000);
    int16_t tmp_h = i2c_smbus_read_byte_data(fd, BMP180_TEMP_H);
    int16_t tmp_l = i2c_smbus_read_byte_data(fd, BMP180_TEMP_H + 1);
    return (tmp_l | tmp_h << 8);
}

uint32_t BMP180::readRawPressure(){
    i2c_smbus_write_byte_data(fd, BMP180_CONTROL, BMP180_READPRESSURE_CMD + (mode << 6));
    switch (mode) {
    case 0:
        usleep(5 * 1000);
        break;
    case 1:
        usleep(8 * 1000);
        break;
    case 2:
        usleep(14 * 1000);
        break;
    case 3:
        usleep(26 * 1000);
        break;    
    }

    uint32_t raw = i2c_smbus_read_byte_data(fd, BMP180_TEMP_H);
    raw <<= 16;
    raw |= i2c_smbus_read_byte_data(fd, BMP180_TEMP_H + 1) << 8;
    raw |= i2c_smbus_read_byte_data(fd, BMP180_TEMP_H + 2);
    raw >>= (8 - mode);
    return raw;
}

int32_t BMP180::computeB5(int32_t UT){
    int32_t X1 = (UT - (int32_t)AC6) * ((int32_t)AC5) >> 15;
    int32_t X2 = ((int32_t)MC << 11) / (X1 + (int32_t)MD);
    return X1 + X2;
}

float BMP180::readTemperature(){
    int32_t UT, B5;
    UT = readRawTemperature();
    B5 = computeB5(UT);
    return static_cast<float>(((B5 + 8) >> 4) / 10.0); //  = (B5 + 8)/2^4
}

int32_t BMP180::readPressure(){
    int32_t UT, UP, p;
    int32_t B3, B4, B5, B6, B7;
    int32_t X1, X2, X3;
    UT = readRawTemperature();
    UP = readRawPressure();   
    B5 = computeB5(UT);
    B6 = B5 - 4000;
    X1 = ((int32_t)B2 * ((B6 * B6) >> 12)) >> 11;
    X2 = ((int32_t)AC2 * B6) >> 11;
    X3 = X1 + X2;
    B3 = ((((int32_t)AC1 * 4 + X3) << mode) + 2) / 4;

    X1 = ((int32_t)AC3 * B6) >> 13;
    X2 = ((int32_t)B1 * ((B6 * B6) >> 12)) >> 16;
    X3 = ((X1 + X2) + 2) >> 2;
    B4 = ((uint32_t)AC4 * (uint32_t)(X3 + 32768)) >> 15;
    B7 = ((uint32_t)UP - B3) * (uint32_t)(50000UL >> mode);

    if (B7 < 0x80000000) {
        p = (B7 * 2) / B4;
    } else {
        p = (B7 / B4) * 2;
    }
    X1 = (p >> 8) * (p >> 8);
    X1 = (X1 * 3038) >> 16;
    X2 = (-7357 * p) >> 16;
    p = p + ((X1 + X2 + (int32_t)3791) >> 4);

    return p;
}

float BMP180::readAltitude(float sealevelPressure){
    float pressure = readPressure();
    float altitude = 44330 * (1.0 - pow(pressure / sealevelPressure, 0.1903));
    return altitude;
}

float BMP180::readSealevelPressure(float altitude_meters){
    float pressure = readPressure();
    return (int32_t)(pressure / pow(1.0 - altitude_meters / 44330, 5.255));
}


void BMP180::prinfCalbrationCoef() const{
    std::cout << "AC1: " << AC1 << std::endl;
    std::cout << "AC2: " << AC2 << std::endl;
    std::cout << "AC3: " << AC3 << std::endl;
    std::cout << "AC4: " << AC4 << std::endl;
    std::cout << "AC5: " << AC5 << std::endl;
    std::cout << "AC6: " << AC6 << std::endl;

    std::cout << "B1: " << B1 << std::endl;
    std::cout << "B2: " << B2 << std::endl;
    
    std::cout << "MB: " << MB << std::endl;
    std::cout << "MC: " << MC << std::endl;
    std::cout << "MD: " << MD << std::endl;
}

void BMP180::reportError(int error, std::string error_info) const{
    std::cerr << "Error! " << error_info << ": " << strerror(error); 
}