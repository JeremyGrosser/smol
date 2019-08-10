#ifndef BOARD_H
#define BOARD_H

#include <platform/uart.h>
#include <platform.h>
#include <stdint.h>
#include <stddef.h>

/*
extern gpio_t P0, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12, P13, P14, P15, P16, P19, P20;
#define BUTTON_A P5
#define BUTTON_B P11
*/

extern gpio_t CONSOLE_RXD, CONSOLE_TXD;
extern uart_t CONSOLE;

int board_init();
void console_write(uint8_t *msg, size_t len);

#endif
