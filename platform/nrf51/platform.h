#ifndef PLATFORM_H
#define PLATFORM_H

#include <nrf.h>
#include <system_nrf51.h>
#include <cmsis_gcc.h>

#include <platform/gpio.h>

#include <stdint.h>

struct exti_s {
	uint32_t num;
	exti_func_t function;
	enum {
		SENSE_NONE  = 0,
		SENSE_HIGH  = 1,
		SENSE_LOW   = 2,
		SENSE_BOTH  = 4,
	} sense;
};

struct pincfg_s {
    enum {
        DIR_IN,
        DIR_OUT,
    } direction;
    enum {
        DRIVE_DISCONNECT,
        DRIVE_LOW,
        DRIVE_HIGH,
    } drive;
    enum {
        PULL_DISABLE,
        PULL_ENABLE, // both
        PULL_UP,
        PULL_DOWN,
    } pull;
};

struct gpio_s {
    uint32_t num;
    pincfg_t config;
	exti_t interrupt;

    uint32_t mask;
};

struct timer_s {
    uint32_t num;
    NRF_TIMER_Type *timer;
};

struct spi_s {
	uint32_t num;

	gpio_t *mosi;
	gpio_t *miso;
	gpio_t *sck;
	gpio_t *nss;

	uint8_t mosi_pad;
	uint8_t miso_pad;
	uint8_t sck_pad;
	uint8_t nss_pad;

	NRF_SPI_Type *spi;
};

struct uart_s {
	uint32_t num;

	gpio_t *txd;
	gpio_t *rxd;

	uint32_t baud;

	NRF_UART_Type *pdev;
};

struct i2c_s {
	uint32_t num;

	gpio_t *scl;
	gpio_t *sda;

	NRF_TWI_Type *twi;
};

void platform_init(void);
uint32_t platform_ticks(void);
void platform_delay(uint32_t ms);

#endif
