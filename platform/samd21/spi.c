#include <platform/spi.h>
#include <platform/gpio.h>
#include <platform.h>
#include "gclk.h"

static uint8_t spi_baud(uint32_t baud) {
	float result;
	result = (((float)SystemCoreClock / (2.0 * (float)baud)) - 1.0);
	return (uint8_t)result;
}

int spi_setup(spi_t *spi) {
	Sercom *sercom;

	sercom = (Sercom *)(((void *)SERCOM0) + (spi->num * 0x400UL));
	spi->sercom = &sercom->SPI;

	gpio_setup(spi->mosi);
	gpio_setup(spi->miso);
	gpio_setup(spi->sck);
	gpio_setup(spi->nss);

	gpio_write(spi->nss, 1);

	GCLK->CLKCTRL.reg = (
			GCLK_CLKCTRL_ID(GCLK_CLKCTRL_ID_SERCOM0_CORE_Val + spi->num) |
			GCLK_CLKCTRL_GEN(GCLK_GEN_MAIN) |
			GCLK_CLKCTRL_CLKEN);

	PM->APBCMASK.reg |= (PM_APBCMASK_SERCOM0 << spi->num);

	// Disable and reset
	spi->sercom->INTENCLR.reg = 0xFF;
	spi->sercom->CTRLA.bit.SWRST = 1;
	while(spi->sercom->SYNCBUSY.reg);

	// MODE0: CPOL=0 CPHA=0
	// MSB first DORD=0
	// No addressing FORM=0
	spi->sercom->CTRLA.bit.MODE = 3;
	spi->sercom->CTRLA.bit.DIPO = spi->miso_pad;
	spi->sercom->CTRLA.bit.FORM = 0;

	if(spi->mosi_pad == 0 && spi->sck_pad == 1) {
		spi->sercom->CTRLA.bit.DOPO = 0;
	}else if(spi->mosi_pad == 2 && spi->sck_pad == 3) {
		spi->sercom->CTRLA.bit.DOPO = 1;
	}else if(spi->mosi_pad == 3 && spi->sck_pad == 1) {
		spi->sercom->CTRLA.bit.DOPO = 2;
	}else if(spi->mosi_pad == 0 && spi->sck_pad == 3) {
		spi->sercom->CTRLA.bit.DOPO = 3;
	}else{
		return -1;
	}

	// 8-bits per character CHSIZE=8
	spi->sercom->CTRLB.reg = (
			SERCOM_SPI_CTRLB_CHSIZE(8) |
			SERCOM_SPI_CTRLB_RXEN);
	while(spi->sercom->SYNCBUSY.bit.CTRLB);

	// Note: Selected baud must be <= (SystemCoreClock / 2)
	spi->sercom->BAUD.bit.BAUD = spi_baud(SystemCoreClock / 2);

	// Enable
	spi->sercom->CTRLA.bit.ENABLE = 1;
	while(spi->sercom->SYNCBUSY.bit.ENABLE);

	spi->sercom->INTENSET.bit.TXC = 1;
	spi->sercom->INTENSET.bit.RXC = 1;
	spi->sercom->INTENSET.bit.DRE = 1;
	//NVIC_EnableIRQ(SERCOM0_IRQn + spi->num);
	
	return 0;
}

void spi_begin(spi_t *spi) {
	gpio_write(spi->nss, 0);
}

uint8_t spi_transfer(spi_t *spi, uint8_t data) {
	uint8_t ret;

	// Wait until data register is empty
	while(!spi->sercom->INTFLAG.bit.DRE);

	// Load a byte
	spi->sercom->DATA.bit.DATA = data;

	// Wait for transmit complete
	while(!spi->sercom->INTFLAG.bit.TXC);

	// Wait for receive complete
	while(!spi->sercom->INTFLAG.bit.RXC);

	// Read a byte
	ret = spi->sercom->DATA.bit.DATA;

	// Clear interrupts
	spi->sercom->INTFLAG.reg = 0xFF;

	return ret;
}

void spi_end(spi_t *spi) {
	gpio_write(spi->nss, 1);
}

void spi_interrupt(spi_t *spi) {
}
