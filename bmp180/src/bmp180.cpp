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
    int16_t ac1_h = i2c_smbus_read_byte_data(fd, BMP180_REG_AC1_H);
    int16_t ac1_l = i2c_smbus_read_byte_data(fd, BMP180_REG_AC1_H + 1);
    AC1 = ac1_l | ac1_h << 8;

    int16_t ac2_h = i2c_smbus_read_byte_data(fd, BMP180_REG_AC2_H);
    int16_t ac2_l = i2c_smbus_read_byte_data(fd, BMP180_REG_AC2_H + 1);
    AC2 = ac2_l | ac2_h << 8;

    int16_t ac3_h = i2c_smbus_read_byte_data(fd, BMP180_REG_AC3_H);
    int16_t ac3_l = i2c_smbus_read_byte_data(fd, BMP180_REG_AC3_H + 1);
    AC3 = ac3_l | ac3_h << 8;

    int16_t ac4_h = i2c_smbus_read_byte_data(fd, BMP180_REG_AC4_H);
    int16_t ac4_l = i2c_smbus_read_byte_data(fd, BMP180_REG_AC4_H + 1);
    AC4 = static_cast<uint16_t>(ac4_l | ac4_h << 8);

    int16_t ac5_h = i2c_smbus_read_byte_data(fd, BMP180_REG_AC5_H);
    int16_t ac5_l = i2c_smbus_read_byte_data(fd, BMP180_REG_AC5_H + 1);
    AC5 = static_cast<uint16_t>(ac5_l | ac5_h << 8);

    int16_t ac6_h = i2c_smbus_read_byte_data(fd, BMP180_REG_AC6_H);
    int16_t ac6_l = i2c_smbus_read_byte_data(fd, BMP180_REG_AC6_H + 1);
    AC6 = static_cast<uint16_t>(ac6_l | ac6_h << 8);

    int16_t b1_h = i2c_smbus_read_byte_data(fd, BMP180_REG_B1_H);
    int16_t b1_l = i2c_smbus_read_byte_data(fd, BMP180_REG_B1_H + 1);
    B1 = b1_l | b1_h << 8;

    int16_t b2_h = i2c_smbus_read_byte_data(fd, BMP180_REG_B2_H);
    int16_t b2_l = i2c_smbus_read_byte_data(fd, BMP180_REG_B2_H + 1);
    B2 = b2_l | b2_h << 8;

    int16_t mb_h = i2c_smbus_read_byte_data(fd, BMP180_REG_MB_H);
    int16_t mb_l = i2c_smbus_read_byte_data(fd, BMP180_REG_MB_H + 1);
    MB = mb_l | mb_h << 8;

    int16_t mc_h = i2c_smbus_read_byte_data(fd, BMP180_REG_MC_H);
    int16_t mc_l = i2c_smbus_read_byte_data(fd, BMP180_REG_MC_H + 1);
    MC = mc_l | mc_h << 8;

    int16_t md_h = i2c_smbus_read_byte_data(fd, BMP180_REG_MD_H);
    int16_t md_l = i2c_smbus_read_byte_data(fd, BMP180_REG_MD_H + 1);
    MD = md_l | md_h << 8;
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