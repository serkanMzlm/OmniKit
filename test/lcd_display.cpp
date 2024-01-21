#include "lcd_2x16.hpp"

int main(){
    LCD lcd;
    std::cout << "Press Enter to exit." << std::endl;
    lcd.cursor();
    lcd.blink();
    lcd.backlight();  
    lcd.writeString();  
    getchar();
    lcd.clear();
    lcd.backlight();
    return 0;
}


