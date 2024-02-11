#include <iostream>
#include <unistd.h> 

#include "gpio_control.hpp"


int main(int argc, char** argv){
    std::cout << "Toplam argüman sayısı: " << argc << std::endl;
    std::cout << "Program adı: " << argv[0] << std::endl;
    if (argc > 2) {
        unexportPin(std::stio(argv[1]));
        usleep(DELAY);
        exportPin(std::stio(argv[1]));
        usleep(DELAY);
        directionPin(std::stio(argv[1]), OUTPUT);
        usleep(DELAY);
        writePin(std::stio(argv[1]), std::stio(argv[0]));
    }

    return 0;
}