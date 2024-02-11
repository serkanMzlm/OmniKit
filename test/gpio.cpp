#include <iostream>
#include <unistd.h> 

#include "gpio_control.hpp"

#define DELAY 10000

int main(int argc, char** argv){
    std::cout << "Toplam argüman sayısı: " << argc << std::endl;
    std::cout << "Program adı: " << argv[0] << std::endl;
    if (argc > 2) {
        unexportPin(std::stoi(argv[1]));
        usleep(DELAY);
        exportPin(std::stoi(argv[1]));
        usleep(DELAY);
        directionPin(std::stoi(argv[1]), OUTPUT);
        usleep(DELAY);
        writePin(std::stoi(argv[1]), std::stoi(argv[0]));
    }

    return 0;
}