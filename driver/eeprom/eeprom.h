#ifndef EEPROM_H
#define EEPROM_H
#include <platform/i2c.h>

#define PAGE_SIZE	32
#define NUM_PAGES	256

struct eeprom_s {
	i2c_t *i2c;
	uint8_t address;

	int page_size;
	int num_pages;
};
typedef struct eeprom_s eeprom_t;

int eeprom_setup(eeprom_t *dev);
int eeprom_page_read(eeprom_t *dev, uint8_t page, uint8_t *buf, size_t len);

#endif
