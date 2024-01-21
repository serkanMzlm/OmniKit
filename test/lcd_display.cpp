#include "lcd_2x16.hpp"

int main(){
    LCD lcd;
    char* number;
    for(int i = 0; i < 100; i++){
        lcd.clear();
        snprintf(number, 10, "Number: %d", i);
        lcd.writeString(number);
    }
    getchar();
    lcd.shutdownLCD();
    return 0;
}


