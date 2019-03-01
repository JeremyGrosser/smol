#include <platform/gpio.h>
#include <board.h>
#include <stdio.h>

#include <sx127x.h>
#include <lib/hexdump.h>

int main(void) {
	int err;

	err = board_init();
	if(err != 0) {
		return 1;
	}

	/*
	usb_open(&USBDEV);
	while(usb_isconfigured(&USBDEV) != 0);
	*/

	uint8_t testdata[] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77};
	while(1) {
		sx127x_transmit(&RFM, testdata, sizeof(testdata));
		platform_delay(1000);
	}

	return 0;
}
