#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

extern "C"{
#include <errno.h>
#include <i2c/smbus.h>
}

#define OK 0
#define FAILED 1

#define LCD_SLAVE_ADDR (0x27)

// Command
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

#define En 0b00000100  // Enable bit
#define Rw 0b00000010  // Read/Write bit
#define Rs 0b00000001  // Register select bit

///////////////////////////////////
//    description (bildirim)     //
///////////////////////////////////
class I2C{
private:
    int fd;
    int i2c_bus = 1;
    char filename[11];
	uint8_t _cols = 16;
	uint8_t _rows = 2;
    uint8_t _addr = LCD_SLAVE_ADDR;
	uint8_t _backlightval = LCD_BACKLIGHT;
    uint8_t _displayfunction;
private:
    void sleep(uint16_t ms_time);
    void write4bits(uint8_t value);
    int expanderWrite(uint8_t data);
    void pulseEnable(uint8_t data);
    void reportError(int error, std::string info = "Errno");
public:
    I2C(void);
    ~I2C();
    int init(const char* filename, int slave_addr = LCD_SLAVE_ADDR);
    void begin();
};

int main(){
    I2C _i2c;
    std::cout << "Press Enter to exit." << std::endl;
    getchar();
    return 0;
}


///////////////////////////////////
//      definition (tanım)       //
///////////////////////////////////
I2C::I2C(void){
    std::cout << "The file has been launched." << std::endl;
    snprintf(filename, 11, "/dev/i2c-%d", i2c_bus);
    if(init(filename, LCD_SLAVE_ADDR)){
        exit(-1);
    }
    begin();
}

I2C::~I2C(){
    close(fd);
}

int I2C::init(const char* filename, int slave_addr){
    fd = open(filename, O_RDWR);
    if(fd < 0){
        reportError(errno, "Failed  to open I2C device.");
        return FAILED;
    }
    if(ioctl(fd, I2C_SLAVE, slave_addr)){
        reportError(errno, "Failed to connect to the I2C device");
        close(fd);
        return FAILED;
    }
    return OK;
}

void I2C::begin(){
    _displayfunction = LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS;
    sleep(50);
    expanderWrite(_backlightval);
    sleep(10000);

    write4bits(0x03 << 4);
    sleep(5);
    write4bits(0x03 << 4);
    sleep(5);
    write4bits(0x03 << 4);
    sleep(5);
    write4bits(0x02 << 4);


}

void I2C::sleep(uint16_t ms_time){
    usleep(ms_time * 1000);
}

void I2C::write4bits(uint8_t value){
    expanderWrite(value);
	pulseEnable(value);
}

int I2C::expanderWrite(uint8_t data){
    if(write(fd, &data, 1) != 1){
        reportError(errno, "Error writing data over I2C");
        return FAILED;
    }
    return OK;
}

void I2C::pulseEnable(uint8_t data){
    expanderWrite(data | En);
    usleep(1);
    expanderWrite(data & ~En);
    usleep(50);
}

void I2C::reportError(int error, std::string info){
    std::cerr << "Error! " << info << " : " << strerror(error);
}