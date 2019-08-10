#include <platform/gpio.h>
#include <board.h>
#include <stdio.h>

int main(void) {
	int err;

	err = board_init();
	if(err != 0) {
		return 1;
	}

    gpio_t LED_ROW1 = {
        .num = 13,
        .config = {
            .direction = DIR_OUT,
            .drive = DRIVE_LOW,
            .pull = PULL_DISABLE,
        },
    };

    gpio_t LED_COL1 = {
        .num = 4,
        .config = {
            .direction = DIR_OUT,
            .drive = DRIVE_LOW,
            .pull = PULL_DISABLE,
        },
    };

    gpio_setup(&LED_ROW1);
    gpio_setup(&LED_COL1);

    gpio_write(&LED_ROW1, 1);
    gpio_write(&LED_COL1, 0);

    printf("smol microbit ready\r\n");

	while(1) {
        __WFI();
        //platform_delay(100);
        //platform_delay(100);
	}

	return 0;
}
