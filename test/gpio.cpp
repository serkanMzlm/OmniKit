#include <iostream>
#include <unistd.h> 

#include "gpio_control.hpp"

#define DELAY 100000

int main(int argc, char** argv){
    std::cout << "total number of arguments: " << argc << std::endl;
    std::cout << "program name: " << argv[0] << std::endl;
    if (argc > 2) {
        std::cout << "pin: " << argv[1] << " value: " << argv[2] << std::endl;
        int pin = atoi(argv[1]);
        int value = atoi(argv[2]);

        unexportPin(pin);
        usleep(DELAY);
        exportPin(pin);
        usleep(DELAY);
        directionPin(pin, OUTPUT);
        usleep(DELAY);
        writePin(pin, value);
        usleep(DELAY);
    }

    return 0;
}