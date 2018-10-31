#ifndef BOARD_H
#define BOARD_H

#include <platform.h>
#include <platform/i2c.h>
#include <platform/uart.h>

extern gpio_t D0, D1, D2, D3, D4, D5, D6, D7, D8, D9, D10, D11, D12, D13, D20, D21;
extern gpio_t A0, A1, A2, A3, A4, A5, AREF;

extern uart_t UART;
extern i2c_t I2C;

#define LED D13
#define LED_ON	0
#define LED_OFF 1

int board_init();
void console_write(uint8_t *msg, size_t len);

#endif
