#ifndef SPI_H
#define SPI_H
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct spi_s spi_t;

int spi_setup(spi_t *spi);
void spi_begin(spi_t *spi);
size_t spi_transfer(spi_t *spi, uint8_t *out, uint8_t *in, size_t len);
void spi_end(spi_t *spi);
void spi_interrupt(spi_t *spi);

#endif
