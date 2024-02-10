#include <iostream>
#include "gpio_control.hpp"

using namespace gpio_control;

int main(int argc, char** argv){
    std::cout << "Toplam argüman sayısı: " << argc << std::endl;
    std::cout << "Program adı: " << argv[0] << std::endl;
    if (argc > 2) {
        exportPin(std::stoi(argv[1]));
        directionPin(std::stoi(argv[1]), OUTPUT);
        writePin(std::stoi(argv[1]), std::stoi(argv[2]));
    }

    return 0;
}