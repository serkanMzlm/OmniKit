#include "lcd_2x16.hpp"

int main(){
    LCD lcd;
    std::cout << "Press Enter to exit." << std::endl;
    lcd.cursor();
    lcd.blink();
    lcd.backlight();  
    lcd.writeString("Hello");
    lcd.setCursor(0, 1);
    lcd.writeString("World");
    getchar();
    lcd.clear();
    lcd.backlight();
    return 0;
}


