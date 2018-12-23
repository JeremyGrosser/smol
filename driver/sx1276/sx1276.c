#include <platform/spi.h>
#include <platform.h>
#include "sx1276.h"

#include <stdio.h>


#define REG_OPMODE		0x01
#define REG_VERSION		0x42

static uint8_t sx1276_readreg(sx1276_t *dev, uint8_t reg) {
	uint8_t out[2];
	uint8_t in[2];

	out[0] = reg & 0x7f;
	out[1] = 0x00;

	spi_transfer(dev->spi, out, in, 2);
	return in[1];
}


static uint8_t sx1276_writereg(sx1276_t *dev, uint8_t reg, uint8_t val) {
	uint8_t out[2];
	uint8_t in[2];

	out[0] = reg | 0x80;
	out[1] = val;

	spi_transfer(dev->spi, out, in, 2);
	return in[1];
}


int sx1276_setup(sx1276_t *dev) {
	uint8_t version;
	uint8_t opmode;

	gpio_setup(dev->reset);
	gpio_write(dev->reset, 0);
	platform_delay(10);
	gpio_write(dev->reset, 1);
	platform_delay(10);

	spi_setup(dev->spi);
	version = sx1276_readreg(dev, REG_VERSION);

	/*
	printf("sx1276: version %d.%d\r\n",
			(version >> 4),
			(version & 0x0F));
			*/

	sx1276_writereg(dev, REG_OPMODE, 0x00);		// SLEEP
	sx1276_writereg(dev, REG_OPMODE, 0x80);		// LoRa mode
	sx1276_writereg(dev, REG_OPMODE, 0x81);		// STANDBY

	return 0;
}
