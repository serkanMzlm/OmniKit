// #include "pmw3901.hpp"
extern "C" {
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
}

uint8_t mode = 0;
uint32_t speed = 500000;
uint8_t bits = 8;

void spi_transfer(int fd, uint8_t *data, int length){
    struct spi_ioc_transfer spi[length];
    int i;
    for(i = 0; i < length; i++){
        memset(&spi[i], 0, sizeof(struct spi_ioc_transfer));
        spi[i].tx_buf = (unsigned long) (data + i);
        spi[i].rx_buf = (unsigned long) (data + i);
        spi[i].len = 1;
        spi[i].speed_hz = speed;
        spi[i].bits_per_word = bits;
    }

    if(ioctl(fd, SPI_IOC_MESSAGE(length), spi) < 0){
        printf("Error transfering data over  SPI bus\n");
        close(fd);
        exit(-1);
    }
}


int main(int argc, char* argv[]){
    int fd;
    uint8_t data[3];
    if ((fd = open("/dev/spidev0.0", O_RDWR)) < 0){
        printf("Open spi device failed!\n");
        return -1;
    }
    if(ioctl(fd, SPI_IOC_WR_MODE, &mode) < 0){
        printf("Error setting the SPI mode\n");
        close(fd);
        return -1;
    }
    if(ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0){
        printf("Error setting the SPI speed\n");
        close(fd);
        return -1;
    }
    if(ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits) < 0){
        printf("Error setting the SPI bits\n");
        close(fd);
        return -1;
    }

    return 0;
}