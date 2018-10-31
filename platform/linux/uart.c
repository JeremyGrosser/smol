#include <platform/uart.h>

int uart_setup(uart_t *uart) {
	return -1;
}

void uart_putc(uart_t *uart, uint8_t c) {
}

uint8_t uart_getc(uart_t *uart) {
	return 0;
}

void uart_write(uart_t *uart, uint8_t *msg, size_t len) {
}

int uart_read(uart_t *uart, uint8_t *msg, size_t len) {
	return -1;
}
