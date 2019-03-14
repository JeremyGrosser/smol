#include <platform/gpio.h>
#include <board.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sx127x.h>
#include <lib/hexdump.h>

#include <lorawan_channels_us915.h>

#ifndef RFM
sx127x_t RFM;
#endif

#ifndef LED_ON
#define LED_ON 1
#define LED_OFF 0
gpio_t LED;
#endif

int main(void) {

	int len;
	int err;

	err = board_init();
	if(err != 0) {
		return 1;
	}

	/*
	eeprom_page_t page;
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
		printf("eeprom: write page failed: %d\r\n", err);
	}

	memset((void *)page.data, 0xAA, EEPROM.page_size);
	err = eeprom_page_read(&EEPROM, &page);
	if(err != 0) { 
		printf("eeprom: read page failed: %d\r\n", err);
	}else{
		hexdump(page.data, EEPROM.page_size);
	}
	*/

	printf("sx127x: version 0x%02X\r\n", sx127x_version(&RFM));

	/*
	usb_open(&USBDEV);
	while(usb_isconfigured(&USBDEV) != 0);
	*/

	// first byte transmitted is packet length, second byte is optional address
	//uint8_t testdata[] = {0x08, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77};
	uint8_t rxbuf[64] = {0};

	setbuf(stdout, NULL);

	uint8_t chan = 0;
	uint8_t dr = 0;

	while(1) {
		gpio_write(&LED, LED_OFF);

		if(RFM.count_packets_rx == 0) {
			err = sx127x_set_channel(&RFM, lorawan_channels_upstream, lorawan_data_rates, chan, dr);
			if(err != 0) {
				printf("sx127x: unable to set channel: %s\r\n", sx127x_get_error(&RFM));
			}

			if(dr == lorawan_channels_upstream[chan].max_dr) {
				chan = (chan + 1) % 3;
				dr = lorawan_channels_upstream[chan].min_dr;
			}else{
				dr++;
			}
		}


		//platform_delay(10);
		/*
		err = sx127x_transmit(&RFM, testdata, sizeof(testdata));
		platform_delay(100);
		*/

		len = sx127x_receive(&RFM, rxbuf, sizeof(rxbuf), 100);
		gpio_write(&LED, LED_ON);
		if(len == -1) {
			// rx timeout
			//printf(".");
			continue;
		}
		if(len < 0) {
			printf("sx127x_receive error: %s\r\n", sx127x_get_error(&RFM));
		}else{
			printf("RX Packets: %lu Errors: %lu\r\n", RFM.count_packets_rx, RFM.count_errors_rx);
			hexdump(rxbuf, len);
		}
	}

	return 0;
}
