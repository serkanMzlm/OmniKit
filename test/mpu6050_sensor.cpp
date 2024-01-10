#include "mpu6050.hpp"
extern "C"{
#include <unistd.h>
}

int main(){
    MPU6050 mpu6050;
    // mpu6050.calibrate();
    std::cout << "\n\n\n\n";
    for(int i = 0; i < 100; i++){
        mpu6050.cleanTerminal();
        mpu6050.printAcceleration();
        mpu6050.printAngularVelocity();
        usleep(10000);
    }
}