#ifndef BOARD_H
#define BOARD_H

#include <platform/gpio.h>
#include <platform.h>
#include <stdint.h>
#include <stddef.h>

extern gpio_t LED;

int board_init();
void console_write(uint8_t *msg, size_t len);

#endif
