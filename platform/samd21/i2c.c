#include <platform/i2c.h>
#include <platform.h>
#include "gclk.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <errno.h>

#define I2C_CLK_MHZ 8
#define I2C_RISE_TIME_NS 300


enum i2c_busstate {
	I2C_BUSSTATE_UNKNOWN	= 0,
	I2C_BUSSTATE_IDLE		= 1,
	I2C_BUSSTATE_OWNER		= 2,
	I2C_BUSSTATE_BUSY		= 3,
};

#define I2C_CMD_STOP		0x03

static uint8_t i2c_baud(uint32_t baudrate) {
	//fSCL = ((fGCLK / 10) + (2 * BAUD) + (fGCLK * tRISE))
	return SystemCoreClock / ( 2 * baudrate) - 5 - ((I2C_CLK_MHZ * I2C_RISE_TIME_NS) / (2 * 1000));
}

void i2c_setup(i2c_t *i2c) {
	Sercom *sercom;
	sercom = (Sercom *)(((void *)SERCOM0) + (i2c->num * 0x400UL));
	i2c->sercom = &sercom->I2CM;

	gpio_setup(i2c->scl);
	gpio_setup(i2c->sda);

	PM->APBCMASK.reg |= (1 << (i2c->num + 2));

	// Fast clock (8MHz)
	GCLK->GENCTRL.reg = (
			GCLK_GENCTRL_ID(GCLK_GEN_I2C_FAST) |
			GCLK_GENCTRL_SRC_OSC8M |
			GCLK_GENCTRL_GENEN);
	GCLK->GENDIV.reg = (
			GCLK_GENDIV_ID(GCLK_GEN_I2C_FAST) |
			GCLK_GENDIV_DIV(8));
	GCLK->CLKCTRL.reg = (
			GCLK_CLKCTRL_ID(GCLK_CLKCTRL_ID_SERCOM0_CORE_Val + i2c->num) |
			GCLK_CLKCTRL_GEN(GCLK_GEN_I2C_FAST) |
			GCLK_CLKCTRL_CLKEN);

	// Slow clock (32KHz)
	GCLK->GENCTRL.reg = (
			GCLK_GENCTRL_ID(GCLK_GEN_I2C_SLOW) |
			GCLK_GENCTRL_SRC_OSCULP32K |
			GCLK_GENCTRL_GENEN);
	GCLK->GENDIV.reg = (
			GCLK_GENDIV_ID(GCLK_GEN_I2C_SLOW) |
			GCLK_GENDIV_DIV(1));

	// Use the same clock as Fast for now
	/*
	GCLK->CLKCTRL.reg = (
			GCLK_CLKCTRL_ID(GCLK_CLKCTRL_ID_SERCOMX_SLOW_Val) |
			GCLK_CLKCTRL_GEN(GCLK_GEN_I2C_FAST) |
			GCLK_CLKCTRL_CLKEN);
			*/

	i2c->sercom->CTRLA.reg = SERCOM_I2CM_CTRLA_SWRST;
	while(i2c->sercom->SYNCBUSY.bit.SWRST);

	i2c->sercom->CTRLA.reg = (
			SERCOM_I2CM_CTRLA_MODE_I2C_MASTER |
			SERCOM_I2CM_CTRLA_SDAHOLD(2) |
			SERCOM_I2CM_CTRLA_RUNSTDBY);
	while(i2c->sercom->SYNCBUSY.bit.ENABLE);

	i2c->sercom->CTRLB.reg = SERCOM_I2CM_CTRLB_SMEN;
	while(i2c->sercom->SYNCBUSY.bit.SYSOP);

	i2c->sercom->BAUD.reg = i2c_baud(100000);
	/*
	i2c->sercom->BAUD.reg = (
			SERCOM_I2CM_BAUD_BAUD(11) |
			SERCOM_I2CM_BAUD_BAUDLOW(22));
			*/

	i2c->sercom->CTRLA.bit.ENABLE = 1;
	while(i2c->sercom->SYNCBUSY.bit.ENABLE);

	// Force into idle bus state
	i2c->sercom->STATUS.bit.BUSSTATE = 1;
	while(i2c->sercom->SYNCBUSY.bit.SYSOP);
}

size_t i2c_read(i2c_t *i2c, uint8_t address, uint8_t *buf, size_t len) {
	size_t i = 0;

	while(i2c->sercom->SYNCBUSY.reg);

	i2c->sercom->CTRLB.bit.ACKACT = 0; // ACK
	i2c->sercom->ADDR.bit.ADDR = SERCOM_I2CM_ADDR_ADDR((address << 1) | 1);
 	while(!i2c->sercom->INTFLAG.bit.MB);

	i2c->sercom->INTFLAG.reg |= SERCOM_I2CM_INTFLAG_MB;
	if(i2c->sercom->STATUS.bit.ARBLOST) {
		return 0;
	}
	if(i2c->sercom->STATUS.bit.RXNACK) {
		return 0;
	}

	for(i = 0; i < len; i++) {
		if(i2c->sercom->STATUS.bit.BUSSTATE != I2C_BUSSTATE_OWNER) {
			return i;
		}

		while(i2c->sercom->SYNCBUSY.reg);

		if(i == (len - 1)) {
			// last byte, issue NACK/STOP
			i2c->sercom->CTRLB.bit.ACKACT = 1;
			i2c->sercom->CTRLB.bit.CMD = 3;
			buf[i] = i2c->sercom->DATA.reg;
			while(i2c->sercom->INTFLAG.bit.SB);
		}else{
			buf[i] = i2c->sercom->DATA.reg;
			while(!i2c->sercom->INTFLAG.bit.SB);
		}
	}

	return i;
}

size_t i2c_write(i2c_t *i2c, uint8_t address, uint8_t *data, size_t len) {
	size_t i;
	while(i2c->sercom->SYNCBUSY.reg);

	i2c->sercom->CTRLB.bit.ACKACT = 0; // ACK
	i2c->sercom->ADDR.bit.ADDR = SERCOM_I2CM_ADDR_ADDR((address << 1));
 	while(!i2c->sercom->INTFLAG.bit.MB);

	i2c->sercom->INTFLAG.reg |= SERCOM_I2CM_INTFLAG_MB;
	if(i2c->sercom->STATUS.bit.ARBLOST) {
		return 0;
	}
	if(i2c->sercom->STATUS.bit.RXNACK) {
		return 0;
	}

	for(i = 0; i < len; i++) {
		if(i2c->sercom->STATUS.bit.BUSSTATE != I2C_BUSSTATE_OWNER) {
			return i;
		}

		while(i2c->sercom->SYNCBUSY.reg);

		if(i == (len - 1)) {
			// last byte, issue NACK/STOP
			i2c->sercom->CTRLB.bit.ACKACT = 1;
			i2c->sercom->CTRLB.bit.CMD = 3;
			i2c->sercom->DATA.reg = data[i];
			while(i2c->sercom->INTFLAG.bit.MB);
		}else{
			i2c->sercom->DATA.reg = data[i];
			while(!i2c->sercom->INTFLAG.bit.MB);
		}
	}

	return i;
}
