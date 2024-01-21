#include "lcd_2x16.hpp"

int main(){
    LCD lcd;
    char number[12];
    for(int i = 0; i < 100; i++){
        lcd.clear();
        snprintf(number, 12, "Number: %d", i);
        lcd.writeString(number);
    }
    getchar();
    lcd.shutdownLCD();
    return 0;
}


