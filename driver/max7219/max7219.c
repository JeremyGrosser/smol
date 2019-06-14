#include "max7219.h"
#include <platform.h>
#include <platform/gpio.h>

int max7219_setup(max7219_t *dev) {
    gpio_setup(dev->cs);
    gpio_setup(dev->clk);
    gpio_setup(dev->din);

    // CS is active-low
    gpio_write(dev->cs, 1);
    gpio_write(dev->clk, 0);
    gpio_write(dev->din, 0);
    return 0;
}

static void max7219_writebyte(max7219_t *dev, uint8_t byte) {
    int i;

    for(i = 0; i < 8; i++) {
        gpio_write(dev->clk, 1);
        platform_delay(1);                          // tCH = 50ns
        gpio_write(dev->din, ((byte >> i) & 1));
        platform_delay(1);                          // tDS = 25ns
        gpio_write(dev->clk, 0);
        platform_delay(1);                          // tCL = 50ns
    }
}

int max7219_set_reg(max7219_t *dev, enum max7219_reg reg, uint8_t *val, size_t len) {
    if(len <= 0 || len > 8) {
        return 1;
    }

    gpio_write(dev->cs, 0);
    while(len > 0) {
        max7219_writebyte(dev, reg);
        max7219_writebyte(dev, val[len-1]);
        len--;
    }
    gpio_write(dev->cs, 1);
    platform_delay(1);  // tCSW = 50ns

    return 0;
}

int max7219_write(max7219_t *dev, uint8_t *seg, size_t len) {
    if(len <= 0 || len > 8) {
        return 1;
    }

    gpio_write(dev->cs, 0);
    while(len > 0) {
        max7219_writebyte(dev, (uint8_t)len);
        max7219_writebyte(dev, seg[len-1]);
        len--;
    }
    gpio_write(dev->cs, 1);
    platform_delay(1);  // tCSW = 50ns

    return 0;
}
