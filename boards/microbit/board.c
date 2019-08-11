#include <platform/uart.h>
#include <platform.h>
#include <board.h>

gpio_t BUTTON_A = {
    .num = 17,
    .config = {
        .direction = DIR_IN,
        .pull = PULL_UP,
    },
};

gpio_t BUTTON_B = {
    .num = 26,
    .config = {
        .direction = DIR_IN,
        .pull = PULL_UP,
    },
};

int board_init() {
    platform_init();
    uart_setup(&CONSOLE);
    return 0;
}
