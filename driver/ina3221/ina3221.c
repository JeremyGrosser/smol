#include <platform/i2c.h>
#include "ina3221.h"

#include <stdint.h>


int ina3221_setup(ina3221_t *dev) {
	i2c_setup(dev->i2c);
	return 0;
}

uint16_t ina3221_read(ina3221_t *dev, uint8_t reg) {
	return (uint16_t)i2c_read(dev->i2c, dev->address | reg);
}

int ina3221_write(ina3221_t *dev, uint8_t reg, uint16_t val) {
	return i2c_write(dev->i2c, dev->address | reg, (uint8_t *)&val, sizeof(uint16_t));
}
