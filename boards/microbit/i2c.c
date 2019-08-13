#include <platform/gpio.h>
#include <platform/i2c.h>
#include <board.h>

// GPIO config as specified in nRF Reference Manual v3.0, Table 258
static gpio_t SDA = {
    .num = 30,
    .config = {
        .direction = DIR_IN,
        .pull = PULL_DISABLE,
        .drive = DRIVE_DISCONNECT,
    },
};

static gpio_t SCL = {
    .num = 0,
    .config = {
        .direction = DIR_IN,
        .pull = PULL_DISABLE,
        .drive = DRIVE_DISCONNECT,
    },
};

/*
 * TWI0 and SPI0 share the same underlying hardware and cannot be used
 * simultaneously. The microbit only exposes one of each, so we use TWI0
 * and SPI1.
 */
i2c_t I2C = {
    .num = 0,
    .scl = &SCL,
    .sda = &SDA,
    .freq_khz = 100,
};
