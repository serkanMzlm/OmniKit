#ifndef __I2C_DRIVER_H__
#define __I2C_DRIVER_H__

#include <stdint.h>
#include <stddef.h>

int i2c_init(int *fd, const char *device, uint8_t address);
int i2c_read_register(int *fd, uint8_t reg, uint8_t *data, size_t len);
int i2c_write_register(int *fd, uint8_t reg, const uint8_t *data, size_t len);
int i2c_smbus_write(int *fd, uint8_t reg, uint8_t value);
int i2c_smbus_read(int *fd, uint8_t reg, uint8_t *value);
void i2c_close(int *fd);

#endif