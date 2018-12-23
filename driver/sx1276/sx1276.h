#ifndef SX1276_H
#define SX1276_H
#include <platform/gpio.h>
#include <platform/spi.h>
#include <stdint.h>

struct sx1276_s {
	spi_t *spi;
	gpio_t *reset;
};
typedef struct sx1276_s sx1276_t;

int sx1276_setup(sx1276_t *dev);

#endif
