#ifndef __LCD_2X16_HPP__
#define __LCD_2X16_HPP__

#include <unistd.h>
#include <cstring>
#include <iostream>

#include "lcd_type.hpp"
///////////////////////////////////
//         description           //
///////////////////////////////////
class LCD{
private:
    int fd;
    int i2c_bus = 1;
    char filename[11];
	uint8_t _cols = 16;
	uint8_t _rows = 2;
    uint8_t _addr = LCD_SLAVE_ADDR;
	uint8_t _backlightval = LCD_BACKLIGHT;
    uint8_t _displayfunction;
    uint8_t _displaymode;
    uint8_t _displaycontrol;
public:
    LCD(void);
    ~LCD();
    int init(const char* filename, int slave_addr = LCD_SLAVE_ADDR);
    void begin();
    void noDisplay();
    void display();
    void clear();
    void home();
    void setcursor(uint8_t col, uint8_t row);
    void noCursor();
    void cursor();
    void noBlink();
    void blink();
    void scrollDisplayLeft();
    void scrollDisplayRight();
    void leftToRight();
    void rightToLeft();
    void autoscroll();
    void noAutoscroll();
    void createChar(uint8_t location, uint8_t charmap[]);
    void noBacklight();
    void backlight();
    void setBacklight(uint8_t new_val);
    int writeString();
private:
    void write4bits(uint8_t value);
    int expanderWrite(uint8_t data);
    void pulseEnable(uint8_t data);
    void command(uint8_t value, uint8_t mode = 0);
private:
    void sleep(uint16_t ms_time);
    void reportError(int error, std::string info = "Errno");
};

#endif
