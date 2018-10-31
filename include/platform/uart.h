/*
 * Serial uart for debugging and whatnot
 */
#ifndef UART_H
#define UART_H
#include <stdint.h>
#include <stddef.h>

typedef struct uart_s uart_t;

int uart_setup(uart_t *uart);
void uart_putc(uart_t *uart, uint8_t c);
uint8_t uart_getc(uart_t *uart);
void uart_write(uart_t *uart, uint8_t *msg, size_t len);
int uart_read(uart_t *uart, uint8_t *msg, size_t len); // maybe add int flags

#endif
