#ifndef MAX7219_H
#define MAX7219_H

#include <platform/gpio.h>
#include <stdint.h>
#include <stddef.h>

/*
 * MAX7219/MAX7221 8-segment LED drivers.
 * This is a bit-banging driver, not using SPI hardware.
 */

struct max7219_s {
    gpio_t *cs;
    gpio_t *clk;
    gpio_t *din;
};
typedef struct max7219_s max7219_t;

enum max7219_reg {
    REG_DECODE_MODE = 0x09,
    REG_INTENSITY   = 0x0A,
    REG_SCAN_LIMIT  = 0x0B,
    REG_SHUTDOWN    = 0x0C,
    REG_TEST        = 0x0F,
};

int max7219_setup(max7219_t *dev);

/*
 * Each MAX7219 has 8 outputs. Up to 8 chips can be chained together.
 *
 * seg is a pointer to an array of 8-bit integers with each bit representing a
 * single segment
 *
 * len is the number of integers in the seg array, which should equal the
 * number of chips that are to be written to.
 */
int max7219_write(max7219_t *dev, uint8_t *seg, size_t len);
int max7219_set_reg(max7219_t *dev, enum max7219_reg reg, uint8_t *val, size_t len);

#endif
