#ifndef __LCD_TYPE_HPP__
#define __LCD_TYPE_HPP__

#define LCD_SLAVE_ADDR (0x27)
#define LCD_ROW 2
#define LCD_COLUMN 16
// Commands
#define LCD_CLEAR_DISPLAY (0x01)
#define LCD_RETURN_HOME (0x02)
#define LCD_ENTRY_MODE_SET (0x04)
#define LCD_DISPLAY_CONTROL (0x08)
#define LCD_CURSOR_SHIFT (0x10) 
#define LCD_FUNCTION_SET (0x20)
#define LCD_SET_CGRAM_ADDR (0x40)
#define LCD_SET_DDRAM_ADDR (0x80)

// Flags for display entry mode 
#define LCD_ENTRY_RIGHT (0x00)
#define LCD_ENTRY_LEFT (0x02)
#define LCD_ENTRY_SHIFT_INCREMENT (0x01)
#define LCD_ENTRY_SHIFT_DECREMENT (0x00)

// Flags for display on/off control
#define LCD_DISPLAY_ON (0x04)
#define LCD_DISPLAY_OFF (0x00)
#define LCD_CURSOR_ON (0x02)
#define LCD_CURSOR_OFF (0x00)
#define LCD_BLINK_ON (0x01)
#define LCD_BLINK_OFF (0x00)

// Flags for display / cursor shift
#define LCD_DISPLAY_MOVE (0x08)
#define LCD_CURSOR_MOVE (0x00)
#define LCD_MOVE_RIGHT (0x04)
#define LCD_MOVE_LEFT (0x00)

// Flags for function set
#define LCD_8_BIT_MODE (0x10)
#define LCD_4_BIT_MODE (0x00)
#define LCD_2_LINE (0x08)
#define LCD_1_LINE (0x00)
#define LCD_5X10_DOTS (0x04)
#define LCD_5X8_DOTS (0x00)

// Flags for backlight control
#define LCD_BACKLIGHT (0x08)
#define LCD_NOBACKLIGHT (0x00)

#define En 0b00000100  // Enable bit
#define Rw 0b00000010  // Read/Write bit
#define Rs 0b00000001  // Register select bit

#endif