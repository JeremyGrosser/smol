#ifndef EEPROM_H
#define EEPROM_H
#include <platform/i2c.h>

struct eeprom_s {
	i2c_t *i2c;
	uint8_t address;
};
typedef struct eeprom_s eeprom_t;

int eeprom_setup(eeprom_t *dev);

#endif
