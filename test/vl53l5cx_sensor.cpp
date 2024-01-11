#include <iostream>
#include <unistd.h>
#include <signal.h>

#include "vl53l5cx.hpp"

void handleCtrlC(int signum) {
    std::cout << "Ctrl+C program terminated" << std::endl;
    exit(signum);
}

int main(){
    VL53L5CX vl53l5cx;
    for(int i = 0; i < 10; i++) {
        std::cout << "\n";
    }
    while (true) {
        vl53l5cx.getRange();
        usleep(10000);
        std::cout << "\033[8A";
    }
    return 0;
}