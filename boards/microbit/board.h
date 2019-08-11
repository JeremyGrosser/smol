#ifndef BOARD_H
#define BOARD_H

#include <platform/uart.h>
#include <platform.h>
#include <stdint.h>
#include <stddef.h>

extern gpio_t BUTTON_A, BUTTON_B;
extern uart_t CONSOLE;

int board_init();
int console_read(uint8_t *msg, size_t len);
void console_write(uint8_t *msg, size_t len);

#endif
