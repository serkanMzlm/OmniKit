#include <iostream>
#include "gpio_control.hpp"

using namespace gpio_control;

int main(int argc, char** argv){

    // argc, komut satırında kaç tane argümanın geçirildiğini belirtir
    // argv, argümanları içeren bir dizi(pointer)dir

    std::cout << "Toplam argüman sayısı: " << argc << std::endl;

    // argv[0], programın adını içerir
    std::cout << "Program adı: " << argv[0] << std::endl;

    // Diğer argümanlar, program adından sonra gelir
    // argv[1], ilk argümanı içerir
    if (argc > 1) {
        std::cout << "İlk argüman: " << argv[1] << std::endl;
    }

    return 0;
}