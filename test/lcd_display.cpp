// #include "lcd_2x16.hpp"

// int main(){
//     LCD lcd;
//     lcd.writeString("Hello World");
//     lcd.setCursor(0, 1);
//     lcd.writeString("---- serkan ---");
//     // lcd.control(1, 0, 1);
//     getchar();
//     lcd.shutdownLCD();
//     return 0;
// }

#include "lcd.hpp"
#include <iostream>

int main() {
    LCD lcd;

    lcd.init();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.printstr("Hello,");
    lcd.setCursor(0, 1);
    lcd.printstr("LCD!");

    usleep(2000000);  // 2 saniye bekle

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.printstr("LCD Test");

    for (int i = 0; i < 16; ++i) {
        lcd.scrollDisplayLeft();
        usleep(500000);  // 0.5 saniye bekle
    }

    return 0;
}

