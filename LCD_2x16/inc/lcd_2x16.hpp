#ifndef __LCD_2X16_HPP__
#define __LCD_2X16_HPP__

#include <iostream>
#include <cstring>
#include <cmath>

#define BACKLIGHT 8
#define DATA 1

namespace LCD_2 {
class LCD{
public:
    LCD(int bus_number = 1);
    ~LCD();
    int initI2c(const char* filename, int addr);
    int setCursor(int x, int y);
    int control(int backlight, int cursor, int blink);
    int writeString(char * text);
    int clear();
    void shutdownLCD();
private:
    void writeCommand(unsigned char ucCMD);
    void reportError(int error, std::string error_info) const;
private:
    int fd;
    char filename[11];
    int backlight = BACKLIGHT;
};
};

#endif
