#include "hexdump.h"
#include <stdio.h>

void hexdump(uint8_t *buf, size_t len) {
	int i;

	for(i = 0; i < len; i++) {
		if((i % 16) == 0) {
			printf("\r\n%08X: ", i);
		}
		printf("%02X ", buf[i]);
	}
	printf("\r\n");
}
