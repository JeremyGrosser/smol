#include <platform/gpio.h>
#include <board.h>

int main(void) {
	int err;

	err = board_init();
	if(err != 0) {
		return 1;
	}

    LED.config.pmux = PMUX_ENABLE;
    LED.config.pmux_function = MUX_PA17E_TCC2_WO1;
	gpio_setup(&LED);

	while(1) {
	}

	return 0;
}
