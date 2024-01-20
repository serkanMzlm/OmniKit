#include "lcd.hpp"

#include <iostream> 
#include <unistd.h> 

LCD::LCD(){

}

LCD::~LCD(){

}

void LCD::oled_init(){
    _oled = true;
    init_priv();
}

void LCD::init(){
    init_priv();
}

void LCD::init_priv(){
    // Wire.begin();
	_display_function = LCD_4_BIT_MODE | LCD_1_LINE | LCD_5X8_DOTS;
	begin(_cols, _rows);
}

void LCD::clear(){
    command(LCD_CLEAR_DISPLAY);
    usleep(2000);
    if(_oled) setCursor(0, 0);
}

void LCD::home(){
    command(LCD_RETURN_HOME);
    usleep(2000);
}

void LCD::setCursor(uint8_t col, uint8_t row){
	int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
    if ( row > _num_lines ) {
		row = _num_lines-1; 
	}

    command(LCD_SET_DDRAM_ADDR | (col + row_offsets[row]));
}

void LCD::noDisplay(){
    _display_control &= ~LCD_DISPLAY_ON;
	command(LCD_DISPLAY_CONTROL | _display_control);
}

void LCD::display(){
    _display_control |= LCD_DISPLAY_ON;
	command(LCD_DISPLAY_CONTROL | _display_control);
}

void LCD::noCursor(){
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

void LCD::scrollDisplayLeft(void) {
	command(LCD_CURSOR_SHIFT | LCD_DISPLAY_MOVE | LCD_MOVE_LEFT);
}
void LCD::scrollDisplayRight(void) {
	command(LCD_CURSOR_SHIFT | LCD_DISPLAY_MOVE | LCD_MOVE_RIGHT);
}

void LCD::leftToRight(void) {
	_display_mode |= LCD_ENTRY_LEFT;
	command(LCD_ENTRY_MODE_SET | _display_mode);
}

void LCD::rightToLeft(void) {
	_display_mode &= ~LCD_ENTRY_LEFT;
	command(LCD_ENTRY_MODE_SET | _display_mode);
}

void LCD::autoscroll(void) {
	_display_mode |= LCD_ENTRY_SHIFT_INCREMENT;
	command(LCD_ENTRY_MODE_SET | _display_mode);
}

void LCD::noAutoscroll(void) {
	_display_mode &= ~LCD_ENTRY_SHIFT_INCREMENT;
	command(LCD_ENTRY_MODE_SET | _display_mode);
}


void LCD::createChar(uint8_t location, uint8_t charmap[]) {
	location &= 0x7; 
	command(LCD_SET_CGRAM_ADDR | (location << 3));
	for (int i=0; i<8; i++) {
		// write(charmap[i]);
	}
}

void LCD::createChar(uint8_t location, const char *charmap) {
	location &= 0x7; 
	command(LCD_SET_CGRAM_ADDR | (location << 3));
	for (int i=0; i<8; i++) {
	    	// write(pgm_read_byte_near(charmap++));
	}
}

void LCD::noBacklight(void) {
	_backlightval = LCD_NOBACKLIGHT;
	expanderWrite(0);
}

void LCD::backlight(void) {
	_backlightval = LCD_BACKLIGHT;
	expanderWrite(0);
}

inline void LCD::command(uint8_t value) {
	send(value, 0);
}

///  
void LCD::send(uint8_t value, uint8_t mode) {
	uint8_t highnib = value & 0xf0;
	uint8_t lownib = (value << 4) & 0xf0;
    write4bits((highnib)|mode);
	write4bits((lownib)|mode); 
}

void LCD::write4bits(uint8_t value) {
	expanderWrite(value);
	pulseEnable(value);
}

void LCD::expanderWrite(uint8_t _data){                                        
	// Wire.beginTransmission(_addr);
	// printIIC((int)(_data) | _backlightval);
	// Wire.endTransmission();   
}

void LCD::pulseEnable(uint8_t _data){
	expanderWrite(_data | En);	// En high
	usleep(1);		// enable pulse must be >450ns
	
	expanderWrite(_data & ~En);	// En low
	usleep(50);		// commands need > 37us to settle
} 


// Alias functions

void LCD::cursor_on(){
	cursor();
}

void LCD::cursor_off(){
	noCursor();
}

void LCD::blink_on(){
	blink();
}

void LCD::blink_off(){
	noBlink();
}

void LCD::load_custom_character(uint8_t char_num, uint8_t *rows){
		createChar(char_num, rows);
}

void LCD::setBacklight(uint8_t new_val){
	if(new_val){
		backlight();		// turn backlight on
	}else{
		noBacklight();		// turn backlight off
	}
}

void LCD::printstr(const char c[]){
	//This function is not identical to the function used for "real" I2C displays
	//it's here so the user sketch doesn't have to be changed 
	// print(c);
}
