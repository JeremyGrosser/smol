#include <platform/i2c.h>
#include <platform.h>
#include "eeprom.h"

int eeprom_setup(eeprom_t *dev) {
	i2c_setup(dev->i2c);
	return 0;
}

int eeprom_read(eeprom_t *dev, uint8_t address, uint8_t *buf, size_t len) {
	return 1;
}
