#include <platform/i2c.h>
#include <platform.h>
#include "eeprom.h"

int eeprom_setup(eeprom_t *dev) {
	i2c_setup(dev->i2c);
	return 0;
}

int eeprom_page_read(eeprom_t *dev, uint8_t page, uint8_t *buf, size_t len) {
	int bytes;

	if(len > dev->page_size) {
		return 1;
	}

	bytes = i2c_read(dev->i2c, dev->address, buf, len);

	if((size_t)bytes == len) {
		return 0;
	}else{
		return bytes;
	}
}
