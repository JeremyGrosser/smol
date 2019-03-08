#ifndef EEPROM_H
#define EEPROM_H
#include <platform/i2c.h>

struct eeprom_s {
	i2c_t *i2c;
	uint8_t address;

	int page_size;
	int num_pages;
};
typedef struct eeprom_s eeprom_t;

struct eeprom_page_s {
	uint16_t address;
	uint8_t *data;
	size_t len;
} __attribute__((packed));
typedef struct eeprom_page_s eeprom_page_t;

int eeprom_setup(eeprom_t *dev);
int eeprom_page_read(eeprom_t *dev, eeprom_page_t *page);
int eeprom_page_write(eeprom_t *dev, eeprom_page_t *page);

#endif
