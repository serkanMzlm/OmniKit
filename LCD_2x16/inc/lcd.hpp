#ifndef __LCD_HPP__
#define __LCD_HPP__

#include <cstdint>
#include "lcd_type.hpp"

class LCD{
public:
    LCD();
    ~LCD();
    void begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5X8_DOTS);
    void clear();
    void home();
    void noDisplay();
    void display();
    void noBlink();
    void blink();
    void noCursor();
    void cursor();
    void scrollDisplayLeft();
    void scrollDisplayRight();
    void printLeft();
    void printRight();
    void leftToRight();
    void rightToLeft();
    void shiftIncrement();
    void shiftDecrement();
    void noBacklight();
    void backlight();
    void autoscroll();
    void noAutoscroll(); 
    void createChar(uint8_t, uint8_t[]);
    void createChar(uint8_t location, const char *charmap);
    void setCursor(uint8_t, uint8_t); 
    void command(uint8_t);
    void init();
    void oled_init();
    void blink_on();						
    void blink_off();       					
    void cursor_on();      	 				
    void cursor_off();      					
    void setBacklight(uint8_t new_val);				
    void load_custom_character(uint8_t char_num, uint8_t *rows);
    void printstr(const char[]);
    uint8_t status();
    void setContrast(uint8_t new_val);
    uint8_t keypad();
    void setDelay(int,int);
    void on();
    void off();
    uint8_t init_bargraph(uint8_t graphtype);
    void draw_horizontal_graph(uint8_t row, uint8_t column, uint8_t len,  uint8_t pixel_col_end);
    void draw_vertical_graph(uint8_t row, uint8_t column, uint8_t len,  uint8_t pixel_col_end);

private:
    void init_priv();
    void send(uint8_t, uint8_t);
    void write4bits(uint8_t);
    void expanderWrite(uint8_t);
    void pulseEnable(uint8_t);
    uint8_t _addr = LCD_SLAVE_ADDR;
    uint8_t _display_function;
    uint8_t _display_control;
    uint8_t _display_mode;
    uint8_t _num_lines;
    bool _oled = false;
    uint8_t _cols = LCD_COLUMN;
    uint8_t _rows = LCD_ROW;
    uint8_t _backlightval = LCD_NOBACKLIGHT;
};

#endif