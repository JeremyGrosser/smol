#ifndef BOARD_H
#define BOARD_H

#include <platform.h>
#include <platform/gpio.h>
#include <platform/uart.h>
#include <stdint.h>
#include <stddef.h>

#include <max7219.h>

extern gpio_t LED;
#define LED_ON	0
#define LED_OFF	1

extern gpio_t CONSOLE_TXD;
extern gpio_t CONSOLE_RXD;
extern uart_t CONSOLE;

extern gpio_t MAX7219_CS;
extern gpio_t MAX7219_CLK;
extern gpio_t MAX7219_DIN;
extern max7219_t MAX7219;

int board_init();
void console_write(uint8_t *msg, size_t len);

#endif
