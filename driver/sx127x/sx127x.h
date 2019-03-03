#ifndef SX127x_H
#define SX127x_H
#include <platform/gpio.h>
#include <platform/spi.h>
#include <stdint.h>

struct sx127x_s {
	spi_t *spi;
	gpio_t *reset;
	gpio_t *dio0;

	char *errmsg;
};
typedef struct sx127x_s sx127x_t;

int sx127x_setup(sx127x_t *dev);
void sx127x_interrupt(void *data);
char *sx127x_get_error(sx127x_t *dev);
uint32_t sx127x_get_frequency(sx127x_t *dev);
uint32_t sx127x_set_frequency(sx127x_t *dev, uint32_t frequency);

uint8_t sx127x_version(sx127x_t *dev);
int sx127x_transmit(sx127x_t *dev, uint8_t *buf, size_t len);
int sx127x_receive(sx127x_t *dev, uint8_t *buf, size_t len);

#endif
