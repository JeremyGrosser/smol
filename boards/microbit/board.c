#include <platform/gpio.h>
#include <platform/uart.h>
#include <platform.h>
#include <board.h>

#include <stdint.h>
#include <stddef.h>

gpio_t CONSOLE_RXD = {
    .num = 25,
    .config = {
        .direction = DIR_IN,
        .drive = DRIVE_LOW,
        .pull = PULL_UP,
    },
};

gpio_t CONSOLE_TXD = {
    .num = 24,
    .config = {
        .direction = DIR_OUT,
        .drive = DRIVE_LOW,
        .pull = PULL_UP,
    },
};

uart_t CONSOLE = {
    .num = 0,
    .txd = &CONSOLE_TXD,
    .rxd = &CONSOLE_RXD,
    .baud = 115200,
};

int board_init() {
	platform_init();
    uart_setup(&CONSOLE);
	return 0;
}

void console_write(uint8_t *msg, size_t len) {
    uart_write(&CONSOLE, msg, len);
}
