#ifndef __LCD_HPP__
#define __LCD_HPP__

#include <cstdint>
#include <unistd.h>

#include "lcd_type.hpp"

class LCD {
public:
    LCD();
    ~LCD();

    void init();
    void clear();
    void home();
    void setCursor(uint8_t col, uint8_t row);
    void noDisplay();
    void display();
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
    void command(uint8_t value);
    void send(uint8_t value, uint8_t mode);
    void write4bits(uint8_t value);
    void printstr(const char c[]);

private:
    void init_priv();
    void expanderWrite(uint8_t _data);
    void pulseEnable(uint8_t _data);

private:
    uint8_t _cols = LCD_COLUMN;
    uint8_t _rows = LCD_ROW;

    uint8_t _display_function;
    uint8_t _display_control;
    uint8_t _display_mode;
    uint8_t _num_lines;
    uint8_t _oled;
    uint8_t _backlightval;
};

#endif
