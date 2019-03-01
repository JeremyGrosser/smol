#include "sx127x.h"

#include <platform.h>
#include <stdbool.h>
#include <stdio.h>


enum {
	RegFifo = 0x00,
	RegOpMode = 0x01,
} sx127x_register;

typedef union {
	struct {
		enum {
			MODE_SLEEP,
			MODE_STANDBY,
			MODE_FSTX,
			MODE_TX,
			MODE_FSRX,
			MODE_RXCONTINUOUS,
			MODE_RXSINGLE,
			MODE_CAD,
		} Mode:3;
		bool LowFrequencyModeOn:1;
		uint8_t :2; // reserved
		bool AccessSharedReg:1;
		bool LongRangeMode:1;
	} bit;
	uint8_t reg;
} __attribute__((packed)) RegOpMode_t;

static uint8_t sx127x_readreg(sx127x_t *dev, uint8_t reg) {
	uint8_t out[2];
	uint8_t in[2];

	out[0] = reg & 0x7f;
	out[1] = 0x00;

	spi_transfer(dev->spi, out, in, 2);
	return in[1];
}

static uint8_t sx127x_writereg(sx127x_t *dev, uint8_t reg, uint8_t val) {
	uint8_t out[2];
	uint8_t in[2];

	out[0] = reg | 0x80;
	out[1] = val;

	spi_transfer(dev->spi, out, in, 2);
	return in[1];
}

static void sx127x_reset(sx127x_t *dev) {
	gpio_setup(dev->reset);
	gpio_setup(dev->dio0);
	spi_setup(dev->spi);

	gpio_write(dev->reset, 0);
	platform_delay(1);
	gpio_write(dev->reset, 1);
	/* 
	 * The datasheet only specifies 5ms wait after reset, but we get odd
	 * RegOpMode values if we don't wait a bit longer.
	 */
	platform_delay(100); 
}

static int sx127x_set_frequency(sx127x_t *dev, double frequency) {
	return 0;
}

int sx127x_setup(sx127x_t *dev) {
	RegOpMode_t opmode;

	dev->errmsg = "No error";

	sx127x_reset(dev);

	opmode.reg = sx127x_readreg(dev, RegOpMode);
	opmode.bit.Mode = MODE_SLEEP;
	sx127x_writereg(dev, RegOpMode, opmode.reg);

	opmode.bit.LongRangeMode = true;
	opmode.bit.AccessSharedReg = false;
	sx127x_writereg(dev, RegOpMode, opmode.reg);

	opmode.bit.Mode = MODE_STANDBY;
	sx127x_writereg(dev, RegOpMode, opmode.reg);

	if(sx127x_readreg(dev, RegOpMode) != opmode.reg) {
		dev->errmsg = "Setting RegOpMode failed";
		return 1;
	}

	sx127x_set_frequency(dev, 915.0);

	return 0;
}

char *sx127x_get_error(sx127x_t *dev) {
	return dev->errmsg;
}

void sx127x_interrupt(void *data) {
	//sx127x_t *dev = (sx127x_t *)data;
}

uint8_t sx127x_version(sx127x_t *dev) {
	return 0;
}

int sx127x_transmit(sx127x_t *dev, uint8_t *buf, size_t len) {
	return 0;
}
