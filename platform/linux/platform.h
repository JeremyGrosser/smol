#ifndef PLATFORM_H
#define PLATFORM_H
#include <stdint.h>

struct gpio_s {
};

struct spi_s {
};

struct uart_s {
};

struct i2c_s {
};

void platform_init(void);
uint32_t platform_ticks(void);
void platform_delay(uint32_t ms);
void platform_wait(void);

#endif
