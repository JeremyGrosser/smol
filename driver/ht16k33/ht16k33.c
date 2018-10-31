#include <platform/i2c.h>
#include <platform.h>
#include "ht16k33.h"

#include <string.h>

static int ht16k33_writereg(ht16k33_t *dev, uint8_t reg, uint8_t value) {
	uint8_t data = (reg | value);
	return i2c_write(dev->i2c, dev->i2c_addr, &data, 1);
}

int ht16k33_setup(ht16k33_t *dev) {
	int err;

	// Enable system oscillator
	err = ht16k33_writereg(dev, HT16K33_REG_SYS, HT16K33_SYS_ENABLE);
	if(err != 0) {
		return err;
	}

	// Enable the display
	err = ht16k33_writereg(dev, HT16K33_REG_DISPLAY, HT16K33_DISPLAY_ON);
	if(err != 0) {
		return err;
	}

	// Set brightness
	err = ht16k33_writereg(dev, HT16K33_REG_DIMMING, (dev->brightness & 0x0F));
	if(err != 0) {
		return err;
	}

	memset(dev->state, 0x00, sizeof(dev->state));
	err = ht16k33_flush(dev);

	return err;
}

void ht16k33_set(ht16k33_t *dev, uint8_t column, uint8_t row) {
	dev->state[column] = row;
}

int ht16k33_flush(ht16k33_t *dev) {
	uint8_t data[17];
	int err;

	// The first byte sent is the write command (0x00), followed by 16 bytes of
	// data to load into Display RAM
	
	// This should never happen, but let's just check the bounds to be sure
	if(sizeof(dev->state) > (sizeof(data) - 1)) {
		return 1;
	}

	memset(data, 0x00, sizeof(data));
	memcpy(data + 1, dev->state, sizeof(dev->state));

	err = i2c_write(dev->i2c, dev->i2c_addr, data, sizeof(data));
	return err;
}
