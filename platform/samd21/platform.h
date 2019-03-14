#ifndef PLATFORM_H
#define PLATFORM_H

#include <samd21.h>
#include <system_samd21.h>
#include <cmsis_gcc.h>

#include <platform/gpio.h>
#include <platform/usbdev.h>

#include <stdint.h>

struct exti_s {
	uint32_t num;
	exti_func_t function;
	void *data;
	enum {
		SENSE_NONE	= EIC_CONFIG_SENSE0_NONE_Val,
		SENSE_RISE	= EIC_CONFIG_SENSE0_RISE_Val,
		SENSE_FALL	= EIC_CONFIG_SENSE0_FALL_Val,
		SENSE_BOTH	= EIC_CONFIG_SENSE0_BOTH_Val,
		SENSE_HIGH	= EIC_CONFIG_SENSE0_HIGH_Val,
		SENSE_LOW	= EIC_CONFIG_SENSE0_LOW_Val,
	} sense;
	enum {
		FILTER_DISABLE	= 0,
		FILTER_ENABLE	= 1,
	} filter;
};

struct pincfg_s {
    enum {
        DIR_IN  = 0,
        DIR_OUT = 1,
    } direction;
    enum {
        DRIVE_LOW   = 0,
        DRIVE_HIGH  = 1,
    } drive;
    enum {
        PULL_DISABLE    = 0,
        PULL_ENABLE     = 1,
    } pull;
    enum {
        PMUX_DISABLE    = 0,
        PMUX_ENABLE     = 1,
    } pmux;
    enum {
        PMUX_A  = 0,
        PMUX_B  = 1,
        PMUX_C  = 2,
        PMUX_D  = 3,
        PMUX_E  = 4,
        PMUX_F  = 5,
        PMUX_G  = 6,
        PMUX_H  = 7,
    } pmux_function;
};

struct gpio_s {
    uint32_t num;
    pincfg_t config;
	exti_t interrupt;

    uint32_t port;
    uint32_t pin;
    uint32_t mask;
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

	SercomSpi *sercom;
};

struct uart_s {
	uint32_t num;

	gpio_t *txd;
	gpio_t *rxd;

	uint8_t tx_pad;
	uint8_t rx_pad;
	uint32_t baud;

	SercomUsart *sercom;
};

struct i2c_s {
	uint32_t num;

	gpio_t *scl;
	gpio_t *sda;

	SercomI2cm *sercom;
};

struct usbdev_s {
	gpio_t *dm;
	gpio_t *dp;

	usb_enumerate_handler enumerate;

	Usb *hw;
	__attribute__((__aligned__(4))) UsbDeviceDescriptor endpoints[4];
	__attribute__((__aligned__(4))) uint8_t udd_ep_out_cache_buffer[2][64];
	__attribute__((__aligned__(4))) uint8_t udd_ep_in_cache_buffer[2][64];
};

void platform_init(void);
uint32_t platform_ticks(void);
void platform_delay(uint32_t ms);
void platform_wait(void);

#endif
