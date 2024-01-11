#include "mpu6050.hpp"

#define I2C_DEVICE_NUMBER 1
#define CALIBRATE true

int count = 0;

int main(){
    MPU6050 mpu6050(I2C_DEVICE_NUMBER);
    if(CALIBRATE){
        mpu6050.calibrate();
    }
    while (count++ < 500) {
        mpu6050.printAccData();
        mpu6050.printGyroData();
        std::cout << "____________________\n";
    }
}

