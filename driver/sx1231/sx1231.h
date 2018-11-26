#ifndef SX1231_H
#define SX1231_H
#include <platform/gpio.h>
#include <platform/spi.h>
#include <stdint.h>

struct sx1231_s {
	spi_t *spi;
	gpio_t *reset;
};
typedef struct sx1231_s sx1231_t;

int sx1231_setup(sx1231_t *dev);

#endif
