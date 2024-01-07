#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#include "i2c_driver.h"

static int fd;

int i2c_init(const char *device, uint8_t address) {
    fd = open(device, O_RDWR);
    if (fd < 0) {
        perror("Could not open the I2C device");
        return -1;
    }

    if (ioctl(fd, I2C_SLAVE, address) < 0) {
        perror("Could not set I2C device address");
        close(fd);
        return -1;
    }

    return 0;
}

int i2c_read_register(int fd, uint8_t reg, uint8_t *data, size_t len) {
    if (write(fd, &reg, 1) != 1) {
        perror("Failed to write register address");
        return -1;
    }

    if (read(fd, data, len) != len) {
        perror("Failed to read from register");
        return -1;
    }

    return 0;
}

int i2c_write_register(int fd, uint8_t reg, const uint8_t *data, size_t len) {
    uint8_t *buffer = (uint8_t *)malloc(len + 1);
    if (!buffer) {
        perror("Memory allocation failed");
        return -1;
    }

    buffer[0] = reg;
    memcpy(buffer + 1, data, len);

    if (write(fd, buffer, len + 1) != (len + 1)) {
        perror("Failed to write to register");
        free(buffer);
        return -1;
    }

    free(buffer);
    return 0;
}

int i2c_smbus_write(int fd, uint8_t reg, uint8_t value) {
    if (i2c_smbus_write_byte_data(fd, reg, value) < 0) {
        perror("Failed to write byte data to register");
        return -1;
    }

    return 0;
}

int i2c_smbus_read(int fd, uint8_t reg, uint8_t *value) {
    int result = i2c_smbus_read_byte_data(fd, reg);
    if (result < 0) {
        perror("Failed to read byte data from register");
        return -1;
    }

    *value = result;
    return 0;
}

void i2c_close(int fd) {
    close(fd);
}