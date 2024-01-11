#include "mpu6050.hpp"
#include <iostream>
#include <unistd.h>
#include <signal.h>

// Ctrl+C sinyali alındığında çalışacak işlev
void handleCtrlC(int signum) {
    std::cout << "Ctrl+C tuş kombinasyonu alındı. Program sonlandırılıyor." << std::endl;
    exit(signum);
}

int main() {
    MPU6050 mpu6050;

    std::cout << "\n\n\n\n\n\n\n\n\n";

    signal(SIGINT, handleCtrlC);

    while (true) {
        mpu6050.cleanTerminal();
        mpu6050.printAcceleration();
        mpu6050.printAngularVelocity();

        usleep(10000);
    }

    return 0;
}
