#include <platform/spi.h>

int spi_setup(spi_t *spi) {
	return -1;
}

void spi_begin(spi_t *spi) {
}

size_t spi_transfer(spi_t *spi, uint8_t *out, uint8_t *in, size_t len) {
	return 0;
}

void spi_end(spi_t *spi) {
}

void spi_interrupt(spi_t *spi) {
}
