#include <platform/gpio.h>
#include <board.h>
#include <stdio.h>

gpio_t LED = {};

int board_init() {
	platform_init();
	return 0;
}

void console_write(uint8_t *msg, size_t len) {
	fwrite(msg, 1, len, stdout);
	fflush(stdout);
}
