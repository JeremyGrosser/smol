#include <platform/uart.h>
#include <platform.h>
#include <board.h>

int board_init() {
    platform_init();
    uart_setup(&CONSOLE);
    return 0;
}
