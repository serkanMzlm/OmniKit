#include "lcd_2x16.hpp"

#define DELAY 100000

int main(){
    LCD lcd;
    char number[12];
    for(int i = 0; i < 100; i++){
        lcd.clear();
        lcd.setCursor(0, 1);
        snprintf(number, 12, "Number: %d", i);
        lcd.writeString(number);
        usleep(DELAY);
    }
    lcd.clear();
    getchar();
    lcd.shutdownLCD();
    return 0;
}


