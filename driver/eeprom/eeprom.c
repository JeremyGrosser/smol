#include <platform/i2c.h>
#include <platform.h>
#include "eeprom.h"

#include <stdio.h>


int eeprom_setup(eeprom_t *dev) {
	i2c_setup(dev->i2c);
	return 0;
}

int eeprom_page_read(eeprom_t *dev, eeprom_page_t *page) {
	int bytes;
	size_t len;

	len = (dev->page_size + sizeof(page->address));
	bytes = i2c_read(dev->i2c, dev->address, (uint8_t *)page, len);

	if((size_t)bytes == page->len) {
		return 0;
	}else{
		return (int)bytes;
	}
}

int eeprom_page_write(eeprom_t *dev, eeprom_page_t *page) {
	int bytes;
	size_t len;

	len = (dev->page_size + sizeof(page->address));
	bytes = i2c_write(dev->i2c, dev->address, (uint8_t *)page, len);

	if((size_t)bytes == len) {
		return 0;
	}else{
		return bytes;
	}
}
