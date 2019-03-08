#include <platform/gpio.h>
#include <board.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sx127x.h>
#include <lib/hexdump.h>

int main(void) {
	eeprom_page_t page;

	int err;
	int len;

	err = board_init();
	if(err != 0) {
		return 1;
	}

	page.address = 0x0000;
	page.data = malloc(EEPROM.page_size);
	if(page.data == NULL) {
		printf("malloc failed, panic\r\n");
		return 1;
	}
	memset((void *)page.data, 0, EEPROM.page_size);
	strcpy((char *)page.data, "\xDE\xAD\xBE\xEF\xCA\xFE\x00");

	err = eeprom_page_write(&EEPROM, &page);
	if(err != 0) {
		printf("eeprom: write page failed\r\n");
	}

	err = eeprom_page_read(&EEPROM, &page);
	if(err != 0) {
		printf("eeprom: read page failed\r\n");
	}else{
		hexdump(page.data, EEPROM.page_size);
	}

	printf("sx127x: version 0x%02X\r\n", sx127x_version(&RFM));
	printf("sx127x: frequency set %lu\r\n", sx127x_set_frequency(&RFM, 903900000));

	/*
	usb_open(&USBDEV);
	while(usb_isconfigured(&USBDEV) != 0);
	*/

	// first byte transmitted is packet length, second byte is optional address
	uint8_t testdata[] = {0x08, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77};
	uint8_t rxbuf[64] = {0};

	setbuf(stdout, NULL);

	while(1) {
		gpio_toggle(&LED);
		platform_delay(1000);
		/*
		err = sx127x_transmit(&RFM, testdata, sizeof(testdata));
		platform_delay(100);

		len = sx127x_receive(&RFM, rxbuf, sizeof(rxbuf));
		if(len == -1) {
			// rx timeout
			printf(".");
			continue;
		}
		if(len < 0) {
			printf("sx127x_receive error: %s\r\n", sx127x_get_error(&RFM));
		}else{
			hexdump(rxbuf, len);
		}
		*/
	}

	return 0;
}
