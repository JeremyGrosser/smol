#include "sx127x.h"
#include "sx127x_internal.h"

#include <platform/spi.h>

uint8_t sx127x_readreg(sx127x_t *dev, enum sx127x_register reg) {
	uint8_t out[2];
	uint8_t in[2];

	out[0] = reg & 0x7f;
	out[1] = 0x00;

	spi_transfer(dev->spi, out, in, 2);
	return in[1];
}

uint8_t sx127x_writereg(sx127x_t *dev, enum sx127x_register reg, uint8_t val) {
	uint8_t out[2];
	uint8_t in[2];

	out[0] = reg | 0x80;
	out[1] = val;

	spi_transfer(dev->spi, out, in, 2);
	return in[1];
}

/* Get transceiver frequency in Hz */
uint32_t sx127x_get_frequency(sx127x_t *dev) {
	uint32_t frf;

	frf = (sx127x_readreg(dev, RegFrMsb) << 16);
	frf |= (sx127x_readreg(dev, RegFrMid) << 8);
	frf |= sx127x_readreg(dev, RegFrLsb);

	return (frf * SX127X_FSTEP);
}

/* Set transciever frequency in Hz. Must be in SLEEP mode. */
uint32_t sx127x_set_frequency(sx127x_t *dev, uint32_t frequency) {
	uint32_t frf;
	frf = (frequency / SX127X_FSTEP);

	sx127x_writereg(dev, RegFrMsb, ((frf >> 16) & 0xFF));
	sx127x_writereg(dev, RegFrMid, ((frf >> 8) & 0xFF));
	sx127x_writereg(dev, RegFrLsb, frf & 0xFF);

	return sx127x_get_frequency(dev);
}
