#ifndef I2C_H
#define I2C_H
#include <stdint.h>
#include <stddef.h>

typedef struct i2c_s i2c_t;

int i2c_setup(i2c_t *i2c);
int i2c_write(i2c_t *i2c, uint8_t address, uint8_t *data, size_t len);
int i2c_read(i2c_t *i2c, uint8_t address, uint8_t *data, size_t len);

int i2c_reg_write(i2c_t *i2c, uint8_t address, uint8_t reg, uint8_t val);
int i2c_reg_read(i2c_t *i2c, uint8_t address, uint8_t reg, uint8_t *val);

#endif
