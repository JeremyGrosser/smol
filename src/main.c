#include <platform/gpio.h>
#include <board.h>

#include <string.h>

int main(void) {
	int err;
    uint8_t segments[4];

	err = board_init();
	if(err != 0) {
		return 1;
	}

	gpio_setup(&LED);

    memset(segments, 0xFF, 4);
    max7219_set_reg(&MAX7219, REG_SCAN_LIMIT, segments, 4);
    memset(segments, 0x01, 4);
    max7219_set_reg(&MAX7219, REG_INTENSITY, segments, 4);

    // Alternately interleave segments
	while(1) {
		gpio_write(&LED, 1);
        memset(segments, 0xAA, 4);
        max7219_write(&MAX7219, segments, 4);
		platform_delay(100);

		gpio_write(&LED, 0);
        memset(segments, 0x55, 4);
        max7219_write(&MAX7219, segments, 4);
		platform_delay(100);
	}

	return 0;
}
