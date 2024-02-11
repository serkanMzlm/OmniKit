#include <iostream>
#include "gpio_control.hpp"

using namespace gpio_control;

int main(int argc, char** argv){
    std::cout << "Toplam argüman sayısı: " << argc << std::endl;
    std::cout << "Program adı: " << argv[0] << std::endl;
    if (argc > 2) {
        unexportPin(17);
        exportPin(17);
        directionPin(17, OUTPUT);
        writePin(17, 0);
    }

    return 0;
}