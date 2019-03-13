#ifndef SX127x_H
#define SX127x_H
#include <platform/gpio.h>
#include <platform/spi.h>
#include <stdint.h>
#include "lorawan.h"

struct sx127x_s {
	spi_t *spi;
	gpio_t *reset;
	gpio_t *dio0;

	char *errmsg;

	bool rx_done;

	uint32_t count_packets_rx;
	uint32_t count_errors_rx;
};
typedef struct sx127x_s sx127x_t;

int sx127x_setup(sx127x_t *dev);
void sx127x_interrupt(void *data);
char *sx127x_get_error(sx127x_t *dev);

int sx127x_set_channel(sx127x_t *dev, lorawan_channel_t *channels, lorawan_data_rate_t *data_rates, uint8_t chan_num, uint8_t dr_num);

uint8_t sx127x_version(sx127x_t *dev);
int sx127x_transmit(sx127x_t *dev, uint8_t *buf, size_t len);
int sx127x_receive(sx127x_t *dev, uint8_t *buf, size_t len, uint32_t timeout_ms);
void sx127x_modem_config(sx127x_t *dev, uint8_t sf, uint8_t bw, uint8_t cr, uint8_t syncword);

#endif
