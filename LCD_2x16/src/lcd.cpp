#include "lcd.hpp"
#include <iostream>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

LCD::LCD() {
}

LCD::~LCD() {
}

void LCD::init() {
    init_priv();
}

void LCD::init_priv() {
    _display_function = LCD_4_BIT_MODE | LCD_1_LINE | LCD_5X8_DOTS;
    // begin(_cols, _rows);
}

void LCD::clear() {
    command(LCD_CLEAR_DISPLAY);
    usleep(2000);
    setCursor(0, 0);
}

void LCD::home() {
    command(LCD_RETURN_HOME);
    usleep(2000);
}

void LCD::setCursor(uint8_t col, uint8_t row) {
    int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
    if (row > _num_lines) {
        row = _num_lines - 1;
    }

    command(LCD_SET_DDRAM_ADDR | (col + row_offsets[row]));
}

void LCD::noDisplay() {
    _display_control &= ~LCD_DISPLAY_ON;
    command(LCD_DISPLAY_CONTROL | _display_control);
}

void LCD::display() {
    _display_control |= LCD_DISPLAY_ON;
    command(LCD_DISPLAY_CONTROL | _display_control);
}

void LCD::noCursor() {
    _display_control &= ~LCD_CURSOR_ON;
    command(LCD_DISPLAY_CONTROL | _display_control);
}

void LCD::cursor() {
    _display_control |= LCD_CURSOR_ON;
    command(LCD_DISPLAY_CONTROL | _display_control);
}

void LCD::noBlink() {
    _display_control &= ~LCD_BLINK_ON;
    command(LCD_DISPLAY_CONTROL | _display_control);
}

void LCD::blink() {
    _display_control |= LCD_BLINK_ON;
    command(LCD_DISPLAY_CONTROL | _display_control);
}

void LCD::scrollDisplayLeft() {
    command(LCD_CURSOR_SHIFT | LCD_DISPLAY_MOVE | LCD_MOVE_LEFT);
}

void LCD::scrollDisplayRight() {
    command(LCD_CURSOR_SHIFT | LCD_DISPLAY_MOVE | LCD_MOVE_RIGHT);
}

void LCD::leftToRight() {
    _display_mode |= LCD_ENTRY_LEFT;
    command(LCD_ENTRY_MODE_SET | _display_mode);
}

void LCD::rightToLeft() {
    _display_mode &= ~LCD_ENTRY_LEFT;
    command(LCD_ENTRY_MODE_SET | _display_mode);
}

void LCD::autoscroll() {
    _display_mode |= LCD_ENTRY_SHIFT_INCREMENT;
    command(LCD_ENTRY_MODE_SET | _display_mode);
}

void LCD::noAutoscroll() {
    _display_mode &= ~LCD_ENTRY_SHIFT_INCREMENT;
    command(LCD_ENTRY_MODE_SET | _display_mode);
}

void LCD::createChar(uint8_t location, uint8_t charmap[]) {
    location &= 0x7;
    command(LCD_SET_CGRAM_ADDR | (location << 3));
    for (int i = 0; i < 8; i++) {
        write4bits(charmap[i]);
    }
}

void LCD::noBacklight() {
    _backlightval = LCD_NOBACKLIGHT;
    expanderWrite(0);
}

void LCD::backlight() {
    _backlightval = LCD_BACKLIGHT;
    expanderWrite(0);
}

void LCD::command(uint8_t value) {
    send(value, 0);
}

void LCD::send(uint8_t value, uint8_t mode) {
    uint8_t highnib = value & 0xf0;
    uint8_t lownib = (value << 4) & 0xf0;
    write4bits((highnib) | mode);
    write4bits((lownib) | mode);
}

void LCD::write4bits(uint8_t value) {
    expanderWrite(value);
    pulseEnable(value);
}

void LCD::expanderWrite(uint8_t _data) {
    int file;
    char filename[20];
    sprintf(filename, "/dev/i2c-1");  // I2C-1 kullanılıyorsa, i2c-0 kullanılıyorsa "/dev/i2c-0" olmalı

    if ((file = open(filename, O_RDWR)) < 0) {
        std::cerr << "Failed to open the i2c bus" << std::endl;
        return;
    }

    if (ioctl(file, I2C_SLAVE, LCD_SLAVE_ADDR) < 0) {
        std::cerr << "Failed to acquire bus access and/or talk to slave." << std::endl;
        close(file);
        return;
    }

    char buf[1] = { _data | _backlightval };
    if (write(file, buf, 1) != 1) {
        std::cerr << "Error writing to i2c slave" << std::endl;
    }

    close(file);
}

void LCD::printstr(const char c[]) {
    for (int i = 0; c[i] != '\0'; ++i) {
        send(c[i], Rs);
    }
}

void LCD::pulseEnable(uint8_t _data) {
    expanderWrite(_data | En);    // En high
    usleep(1);                     // Enable pulse must be >450ns

    expanderWrite(_data & ~En);   // En low
    usleep(50);                    // Commands need > 37us to settle
}
