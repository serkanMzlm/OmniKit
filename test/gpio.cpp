#include <iostream>
#include <unistd.h> 

#include "gpio_control.hpp"

#define DELAY 10000

int main(int argc, char** argv){
    std::cout << "Toplam argüman sayısı: " << argc << std::endl;
    std::cout << "Program adı: " << argv[0] << std::endl;
    if (argc > 2) {
        int pin = atoi(argv[1].c_str());
        int value = atoi(argv[2].c_str());

        unexportPin(pin);
        usleep(DELAY);
        exportPin(pin);
        usleep(DELAY);
        directionPin(pin, OUTPUT);
        usleep(DELAY);
        writePin(pin, value);
    }

    return 0;
}