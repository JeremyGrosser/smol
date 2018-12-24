#ifndef SX1276_H
#define SX1276_H
#include <platform/gpio.h>
#include <platform/spi.h>
#include <stdint.h>

struct sx1276_s {
	spi_t *spi;
	gpio_t *reset;
	gpio_t *dio0;
};
typedef struct sx1276_s sx1276_t;

int sx1276_setup(sx1276_t *dev);
int sx1276_set_frequency(sx1276_t *dev, float mhz);
void sx1276_interrupt(void *data);
void sx1276_dump_status(sx1276_t *dev);

#endif
