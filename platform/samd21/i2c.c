#include <platform/i2c.h>
#include <platform.h>
#include "gclk.h"

#include <stddef.h>
#include <stdint.h>
#include <errno.h>

#define I2C_RISE_TIME_NS 125

#define I2C_BUSSTATE_UNKNOWN	0
#define I2C_BUSSTATE_IDLE		1
#define I2C_BUSSTATE_OWNER		2
#define I2C_BUSSTATE_BUSY		3

static uint8_t i2c_baud(uint32_t baudrate) {
	return SystemCoreClock / ( 2 * baudrate) - 5 - (((SystemCoreClock / 1000000) * I2C_RISE_TIME_NS) / (2 * 1000));
}

void i2c_setup(i2c_t *i2c) {
	Sercom *sercom;
	sercom = (Sercom *)(((void *)SERCOM0) + (i2c->num * 0x400UL));
	i2c->sercom = &sercom->I2CM;

	gpio_setup(i2c->scl);
	gpio_setup(i2c->sda);

	gpio_write(i2c->scl, 0);
	gpio_write(i2c->sda, 0);

	// Fast clock (1MHz)
	GCLK->GENCTRL.reg = (
			GCLK_GENCTRL_ID(GCLK_GEN_I2C_FAST) |
			GCLK_GENCTRL_SRC_OSC8M |
			GCLK_GENCTRL_GENEN);
	GCLK->GENDIV.reg = (
			GCLK_GENDIV_ID(GCLK_GEN_I2C_FAST) |
			GCLK_GENDIV_DIV(1));
	GCLK->CLKCTRL.reg = (
			GCLK_CLKCTRL_ID(GCLK_CLKCTRL_ID_SERCOM0_CORE_Val + i2c->num) |
			GCLK_CLKCTRL_GEN(GCLK_GEN_I2C_FAST) |
			GCLK_CLKCTRL_CLKEN);

	// Slow clock (32KHz)
	/*
	GCLK->GENCTRL.reg = (
			GCLK_GENCTRL_ID(GCLK_GEN_I2C_SLOW) |
			GCLK_GENCTRL_SRC_OSCULP32K |
			GCLK_GENCTRL_GENEN);
	GCLK->GENDIV.reg = (
			GCLK_GENDIV_ID(GCLK_GEN_I2C_SLOW) |
			GCLK_GENDIV_DIV(1);
	*/

	// Use the same clock as Fast for now
	/*
	GCLK->CLKCTRL.reg = (
			GCLK_CLKCTRL_ID(GCLK_CLKCTRL_ID_SERCOMX_SLOW_Val) |
			GCLK_CLKCTRL_GEN(GCLK_GEN_I2C_FAST) |
			GCLK_CLKCTRL_CLKEN);
			*/

	PM->APBCMASK.reg |= (1 << (i2c->num + 2));

	i2c->sercom->CTRLA.reg = SERCOM_I2CM_CTRLA_SWRST;
	while(i2c->sercom->SYNCBUSY.bit.SWRST);

	i2c->sercom->CTRLA.reg = (
			SERCOM_I2CM_CTRLA_MODE_I2C_MASTER |
			SERCOM_I2CM_CTRLA_LOWTOUTEN);

	// Send ACK automatically when DATA is read
	//i2c->sercom->CTRLB.reg |= SERCOM_I2CM_CTRLB_SMEN;

	i2c->sercom->BAUD.reg = i2c_baud(100000);

	i2c->sercom->CTRLA.bit.ENABLE = 1;
	while(i2c->sercom->SYNCBUSY.bit.ENABLE);

	i2c->sercom->STATUS.reg |= SERCOM_I2CM_STATUS_BUSSTATE(I2C_BUSSTATE_IDLE);
	while(i2c->sercom->SYNCBUSY.bit.SYSOP);
}

int i2c_start(i2c_t *i2c, uint8_t address) {
	while(i2c->sercom->STATUS.bit.BUSSTATE != I2C_BUSSTATE_IDLE && i2c->sercom->STATUS.bit.BUSSTATE != I2C_BUSSTATE_OWNER);

	// generate a start condition
	i2c->sercom->ADDR.reg = (uint32_t)address;

	if(address & 1) {
		// read
		// wait for slave on bus
		while(!i2c->sercom->INTFLAG.bit.SB) {
			// If the slave NACKs, send a stop and return
			if(i2c->sercom->INTFLAG.bit.MB) {
				i2c->sercom->CTRLB.bit.CMD = 3;
				return 1;
			}
		}
	}else{
		// write
		// wait for master on bus
		while(!i2c->sercom->INTFLAG.bit.MB);
	}

	return 0;
}

int i2c_read(i2c_t *i2c, uint8_t address) {
	int err;

	address = (address << 1) | 1;

	err = i2c_start(i2c, address);
	if(err != 0) {
		errno = EIO;
		return -1;
	}

	while(!i2c->sercom->INTFLAG.bit.SB);
	return i2c->sercom->DATA.reg;
}

int i2c_write(i2c_t *i2c, uint8_t address, uint8_t *data, size_t len) {
	int err;
	size_t i;

	address = (address << 1);

	err = i2c_start(i2c, address);
	if(err != 0) {
		errno = EIO;
		return -1;
	}

	for(i = 0; i < len; i++) {
		i2c->sercom->DATA.reg = data[i];
		if(i2c->sercom->STATUS.bit.RXNACK) {
			errno = EIO;
			return i;
		}
		while(!i2c->sercom->INTFLAG.bit.MB);
	}

	// Send a STOP
	i2c->sercom->CTRLB.bit.CMD = 3;

	/*
	while(!i2c->sercom->INTFLAG.bit.MB) {
		if(i2c->sercom->STATUS.bit.BUSERR || i2c->sercom->INTFLAG.bit.ERROR) {
			errno = EIO;
			return -2;
		}
	}
	*/

	return 0;
}
