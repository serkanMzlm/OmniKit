#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "gpio_control.hpp"

#define BUFFER_MAX 4
#define VALUE_MAX 30
#define DIRECTION_MAX 35

typedef enum PinMode { INPUT, OUTPUT };

namespace gpio_control{

int directionPin(int pin_number, PinMode pin_mode){
	int fd;
    char path[DIRECTION_MAX];
	static const char s_directions_str[]  = "in\0out";
	snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/direction", pin_number);
	fd = open(path, O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio direction for writing!\n");
		return(-1);
	}
	if (-1 == write(fd, &s_directions_str[pin_mode ? 0 : 3], 3)) {
		fprintf(stderr, "Failed to set direction!\n");
		return(-1);
	}
	close(fd);
	return(0);
}

int exportPin(int pin_number){
    char buffer[BUFFER_MAX];
    ssize_t bytes_written;
    int fd = open("/sys/class/gpio/export", O_WRONLY);
    if(fd==-1) {
        printf("Failed to open unexport for writing!\n");
        return 1;
    }

    bytes_written = snprintf(buffer,BUFFER_MAX,"%d",pin_number);
    write(fd,buffer,bytes_written);
    close(fd);
    return 0;
}

int unexportPin(int pin_number){
    char buffer[BUFFER_MAX];
    ssize_t bytes_written;
    int fd = open ("/sys/class/gpio/unexport", O_WRONLY);
    if (-1 == fd) {
		fprintf(stderr, "Failed to open unexport for writing!\n");
		return(1);
	}

    bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin_number);
	write(fd, buffer, bytes_written);
	close(fd);
	return(0);
}

int writePin(int pin_number, bool state){
    static const char s_values_str[] = "01";
	char path[VALUE_MAX];
	int fd;
	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin_number);
	fd = open(path, O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio value for writing!\n");
		return(1);
	}

	if (1 != write(fd, &s_values_str[state ? 1 : 0], 1)) {
		fprintf(stderr, "Failed to write value!\n");
		return(1);
	}

	close(fd);
	return(0);
}

int readPin(int pin_number) {
    char path[VALUE_MAX];
    char value_str[3];
    int fd;

    snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin_number);
    fd = open(path, O_RDONLY);
    if (-1 == fd) {
        fprintf(stderr, "Failed to open gpio value for reading!\n");
        return -1;
    }

    if (-1 == read(fd, value_str, 1)) {
        fprintf(stderr, "Failed to read value!\n");
        return -1;
    }

    close(fd);
    return atoi(value_str);
}

}
