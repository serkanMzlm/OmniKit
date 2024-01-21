#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/i2c-dev.h>

#include "lcd_2x16.hpp"

#define PULSE_PERIOD 500
#define CMD_PERIOD 4100

#define LCD_SLAVE_ADDR (0x27)
using namespace LCD_2;
LCD::LCD(int bus_number){
    snprintf(filename, 11, "/dev/i2c-%d", bus_number);
    if(initI2c(filename, LCD_SLAVE_ADDR)) { exit(-1); }
}

LCD::~LCD(){
    close(fd);
}

int LCD::initI2c(const char* filename, int addr){
    fd = open(filename, O_RDWR);
    if(fd < -1){
        reportError(errno, "Could not open the I2C device");
        return -1;
    }
    if(ioctl(fd, I2C_SLAVE, addr) < 0){
        reportError(errno, "Could not open the I2C device address");
        close(fd);
        return -1;
    }

    backlight = BACKLIGHT;
    writeCommand(0x02);
    writeCommand(0x28);
    writeCommand(0x0c);
    writeCommand(0x06);
    writeCommand(0x80);
    clear();
    return 0;
}

int LCD::setCursor(int x, int y){
    unsigned char cCmd;
    if (fd < 0 || x < 0 || x > 15 || y < 0 || y > 1)
		return 1;
    cCmd = (y==0) ? 0x80 : 0xc0;
	cCmd |= x;
	writeCommand(cCmd);
	return 0;
}

int LCD::control(int backlight, int cursor, int blink){
    unsigned char ucCMD = 0xc; // display control

	if (fd < 0)
		return 1;
	this->backlight = (backlight) ? BACKLIGHT : 0;
	if (cursor)
		ucCMD |= 2;
	if (blink)
		ucCMD |= 1;
	writeCommand(ucCMD);
 	
	return 0;
}

int LCD::writeString(char * text){
    unsigned char ucTemp[2];
    int i = 0;

	if (fd < 0 || text == NULL)
		return 1;

	while (i<16 && *text)
	{
		ucTemp[0] = backlight | DATA | (*text & 0xf0);
		write(fd, ucTemp, 1);
		usleep(PULSE_PERIOD);
		ucTemp[0] |= 4; // pulse E
		write(fd, ucTemp, 1);
		usleep(PULSE_PERIOD);
		ucTemp[0] &= ~4;
		write(fd, ucTemp, 1);
		usleep(PULSE_PERIOD);
		ucTemp[0] = backlight | DATA | (*text << 4);
		write(fd, ucTemp, 1);
		ucTemp[0] |= 4; // pulse E
                write(fd, ucTemp, 1);
                usleep(PULSE_PERIOD);
                ucTemp[0] &= ~4;
                write(fd, ucTemp, 1);
                usleep(CMD_PERIOD);
		text++;
		i++;
	}
	return 0;
}

int LCD::clear(){
    if (fd < 0)
        return 1;
    writeCommand(0x0E); // clear the screen
    return 0;
}

void LCD::shutdownLCD(){
    backlight = 0; // turn off backlight
	writeCommand(0x08); // turn off display, cursor and blink	
	close(fd);
	fd = -1;
}

void LCD::writeCommand(unsigned char ucCMD){
    unsigned char uc;
    uc = (ucCMD & 0xf0) | backlight;
    write(fd, &uc, 1);
    usleep(PULSE_PERIOD);
    uc |= 4; 
    write(fd, &uc, 1);
	usleep(PULSE_PERIOD);
	uc &= ~4;
    write(fd, &uc, 1);
	usleep(CMD_PERIOD);
	uc = backlight | (ucCMD << 4);
	write(fd, &uc, 1);
	usleep(PULSE_PERIOD);
    uc |= 4;
    write(fd, &uc, 1);
    usleep(PULSE_PERIOD);
    uc &= ~4;
    write(fd, &uc, 1);
	usleep(CMD_PERIOD);
}

void LCD::reportError(int error, std::string error_info) const{
    std::cerr << "Error! " << error_info << ": " << strerror(error); 
}