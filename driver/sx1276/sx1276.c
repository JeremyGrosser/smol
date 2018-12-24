#include <platform/spi.h>
#include <platform.h>
#include "sx1276.h"

#include <stdio.h>

typedef union {
	struct {
		uint8_t long_range:1;
		uint8_t access_shared:1;
		uint8_t :2;
		uint8_t low_frequency:1;
		uint8_t mode:3;
	} bit;
	uint8_t reg;
} SX1276_OPMODE_Type;
#define SX1276_OPMODE				0x01
#define SX1276_OPMODE_MODE_SLEEP	0
#define SX1276_OPMODE_MODE_STANDBY	1
#define SX1276_OPMODE_MODE_FSTX		2
#define SX1276_OPMODE_MODE_TXCONT	3
#define SX1276_OPMODE_MODE_FSRX		4
#define SX1276_OPMODE_MODE_RXCONT	5
#define SX1276_OPMODE_MODE_RXSINGLE	6
#define SX1276_OPMODE_MODE_CAD		7

#define SX1276_FIFO					0x00
#define SX1276_FIFO_TX_BASE_ADDR	0x0E
#define SX1276_FIFO_RX_BASE_ADDR	0x0F
#define SX1276_FIFO_RX_NB_BYTES		0x13
#define SX1276_FIFO_RX_PTR			0x25


typedef union {
	struct {
		uint8_t bandwidth:4;
		uint8_t coding_rate:3;
		uint8_t implicit_header_mode:1;
		uint8_t spreading_factor:4;
		uint8_t tx_continuous_mode:1;
		uint8_t rx_payload_crc_on:1;
		uint16_t symbol_timeout:10;
		uint16_t preamble_length:16;
		uint16_t payload_length:16;
		uint8_t payload_max_length:8;
		uint8_t freq_hopping_period:8;
	} bit;
	uint8_t reg[8];
} SX1276_MODEM_CONFIG_Type;
#define SX1276_MODEM_CONFIG 0x1D
#define SX1276_MODEM_CONFIG_LENGTH 8
#define SX1276_MODEM_CONFIG_BANDWIDTH_7800	0
#define SX1276_MODEM_CONFIG_BANDWIDTH_10400	1
#define SX1276_MODEM_CONFIG_BANDWIDTH_15600	2
#define SX1276_MODEM_CONFIG_BANDWIDTH_20800	3
#define SX1276_MODEM_CONFIG_BANDWIDTH_31250	4
#define SX1276_MODEM_CONFIG_BANDWIDTH_41700	5
#define SX1276_MODEM_CONFIG_BANDWIDTH_62500	6
#define SX1276_MODEM_CONFIG_BANDWIDTH_125000	7
/* bandwidth >= 250KHz not supported in the lower band (169 MHz) */
#define SX1276_MODEM_CONFIG_BANDWIDTH_250000	8
#define SX1276_MODEM_CONFIG_BANDWIDTH_500000	9
#define SX1276_MODEM_CONFIG_CODING_RATE_4_5	1
#define SX1276_MODEM_CONFIG_CODING_RATE_4_6	2
#define SX1276_MODEM_CONFIG_CODING_RATE_4_7	3
#define SX1276_MODEM_CONFIG_CODING_RATE_4_8	4
#define SX1276_MODEM_CONFIG_SPREADING_FACTOR_64		6
#define SX1276_MODEM_CONFIG_SPREADING_FACTOR_128	7
#define SX1276_MODEM_CONFIG_SPREADING_FACTOR_256	8
#define SX1276_MODEM_CONFIG_SPREADING_FACTOR_512	9
#define SX1276_MODEM_CONFIG_SPREADING_FACTOR_1024	10
#define SX1276_MODEM_CONFIG_SPREADING_FACTOR_2048	11
#define SX1276_MODEM_CONFIG_SPREADING_FACTOR_4096	12

typedef union {
	struct {
		uint32_t frf:24;
	} bit;
	uint8_t reg[3];
} SX1276_FRF_Type;
#define SX1276_FRF 0x06
#define SX1276_FRF_LENGTH 3
#define SX1276_FXOSC 32000000.0
#define SX1276_FSTEP (SX1276_FXOSC / (1 << 19))

typedef union {
	struct {
		uint8_t pa_select:1;
		uint8_t max_power:3;
		uint8_t output_power:4;
	} bit;
	uint8_t reg;
} SX1276_PA_CONFIG_Type;
#define SX1276_PA_CONFIG 0x09
#define SX1276_PA_CONFIG_PA_SELECT_RFO		0
#define SX1276_PA_CONFIG_PA_SELECT_BOOST	1

typedef union {
	struct {
		uint8_t dio0:2;
		uint8_t dio1:2;
		uint8_t dio2:2;
		uint8_t dio3:2;
		uint8_t dio4:2;
		uint8_t dio5:2;
		uint8_t :3;
		uint8_t map_preamble_detect:1;
	} bit;
	uint8_t reg[2];
} SX1276_DIO_MAPPING_Type;
#define SX1276_DIO_MAPPING 0x40
#define SX1276_DIO_MAPPING_LENGTH 2
#define SX1276_DIO_MAPPING_LORA_DIO0_RXDONE		0
#define SX1276_DIO_MAPPING_LORA_DIO0_TXDONE		1
#define SX1276_DIO_MAPPING_LORA_DIO0_CADDONE	2
#define SX1276_DIO_MAPPING_LORA_DIO1_RXTIMEOUT	0
#define SX1276_DIO_MAPPING_LORA_DIO1_FHSSCHANGECHANNEL 1
#define SX1276_DIO_MAPPING_LORA_DIO1_CADDETECT	2
#define SX1276_DIO_MAPPING_LORA_DIO2_FHSSCHANGECHANNEL 0
#define SX1276_DIO_MAPPING_LORA_DIO3_CADDONE	0
#define SX1276_DIO_MAPPING_LORA_DIO3_VALIDHEADER 1
#define SX1276_DIO_MAPPING_LORA_DIO3_PAYLOADCRCERROR 2
#define SX1276_DIO_MAPPING_LORA_DIO4_CADDETECT	0
#define SX1276_DIO_MAPPING_LORA_DIO4_PLLLOCK	1
#define SX1276_DIO_MAPPING_LORA_DIO5_MODEREADY	0
#define SX1276_DIO_MAPPING_LORA_DIO5_CLKOUT		1

typedef union {
	struct {
		uint8_t rx_timeout:1;
		uint8_t rx_done:1;
		uint8_t payload_crc_error:1;
		uint8_t valid_header:1;
		uint8_t tx_done:1;
		uint8_t cad_done:1;
		uint8_t fhss_change_channel:1;
		uint8_t cad_detected:1;
	} bit;
	uint8_t reg;
} SX1276_IRQFLAGS_Type;
#define SX1276_IRQFLAGS_MASK	0x11
#define SX1276_IRQFLAGS			0x12


typedef union {
	struct {
		uint8_t rx_coding_rate:3;
		uint8_t modem_clear:1;
		uint8_t header_valid:1;
		uint8_t rx_ongoing:1;
		uint8_t sync:1;
		uint8_t detect:1;
	} bit;
	uint8_t reg;
} SX1276_MODEM_STATUS_Type;
#define SX1276_MODEM_STATUS 0x18

static uint8_t sx1276_readreg(sx1276_t *dev, uint8_t reg) {
	uint8_t out[2];
	uint8_t in[2];

	out[0] = reg & 0x7f;
	out[1] = 0x00;

	spi_transfer(dev->spi, out, in, 2);
	return in[1];
}


static uint8_t sx1276_writereg(sx1276_t *dev, uint8_t reg, uint8_t val) {
	uint8_t out[2];
	uint8_t in[2];

	out[0] = reg | 0x80;
	out[1] = val;

	spi_transfer(dev->spi, out, in, 2);
	return in[1];
}

static int sx1276_readfifo(sx1276_t *dev, uint8_t *buf, size_t len) {
	int i;
	int psize;

	psize = sx1276_readreg(dev, SX1276_FIFO_RX_NB_BYTES);
	if(psize > len) {
		/* received packet was larger than buffer */
		return -1;
	}

	/* reset rx pointer to the beginning of the rx fifo */
	sx1276_writereg(dev, SX1276_FIFO_RX_PTR, sx1276_readreg(dev, SX1276_FIFO_RX_BASE_ADDR));

	for(i = 0; i < psize; i++) {
		buf[i] = sx1276_readreg(dev, SX1276_FIFO);
	}

	/* reset rx pointer to the beginning of the rx fifo */
	sx1276_writereg(dev, SX1276_FIFO_RX_PTR, sx1276_readreg(dev, SX1276_FIFO_RX_BASE_ADDR));

	return psize;
}


int sx1276_set_frequency(sx1276_t *dev, float mhz) {
	SX1276_FRF_Type frf;
	int i;

	frf.bit.frf = ((mhz * 1000000.0) / SX1276_FSTEP);
	for(i = 0; i < SX1276_FRF_LENGTH; i++) {
		sx1276_writereg(dev, SX1276_FRF, frf.reg[i]);
	}

	return 0;
}


int sx1276_setup(sx1276_t *dev) {
	SX1276_OPMODE_Type opmode;
	SX1276_DIO_MAPPING_Type diomap;
	//903.9 - SF7BW125 to SF10BW125
	SX1276_MODEM_CONFIG_Type modem_config = {
		.bit = {
			.bandwidth = SX1276_MODEM_CONFIG_BANDWIDTH_125000,
			.coding_rate = SX1276_MODEM_CONFIG_CODING_RATE_4_5,
			.spreading_factor = 7,
			.tx_continuous_mode = 0,
			.rx_payload_crc_on = 0,
			.symbol_timeout = 0x0064,
			.preamble_length = 0x0008,
			.payload_length = 0x01ff,
			.payload_max_length = 0xff,
			.freq_hopping_period = 0,
		},
	};
	/*
	SX1276_PA_CONFIG_Type pa_config = {
		.bit = {
			.pa_select = SX1276_PA_CONFIG_PA_SELECT_RFO,
			.max_power = 0x04,
			.output_power = 0x0f,
		},
	};
	*/
	SX1276_IRQFLAGS_Type irqmask;

	int i;

	spi_setup(dev->spi);

	gpio_setup(dev->reset);
	gpio_write(dev->reset, 0);
	platform_delay(10);
	gpio_write(dev->reset, 1);
	platform_delay(10);

	opmode.reg = sx1276_readreg(dev, SX1276_OPMODE);
	opmode.bit.long_range = 1;
	opmode.bit.mode = SX1276_OPMODE_MODE_SLEEP;
	sx1276_writereg(dev, SX1276_OPMODE, opmode.reg);
	platform_delay(10);

	if(sx1276_readreg(dev, SX1276_OPMODE) != opmode.reg) {
		printf("sx1276: setting opmode failed\r\n");
		return 1;
	}

	/* only configure DIO0 interrupts, we don't have the rest connected */
	gpio_setup(dev->dio0);
	diomap.reg[0] = sx1276_readreg(dev, SX1276_DIO_MAPPING);
	diomap.bit.dio0 = SX1276_DIO_MAPPING_LORA_DIO0_RXDONE;
	sx1276_writereg(dev, SX1276_DIO_MAPPING, diomap.reg[0]);

	/* use the entire 256 byte FIFO for tx/rx, but not both at the same time */
	sx1276_writereg(dev, SX1276_FIFO_TX_BASE_ADDR, 0);
	sx1276_writereg(dev, SX1276_FIFO_RX_BASE_ADDR, 0);
	sx1276_writereg(dev, SX1276_FIFO_RX_PTR, 0);

	/*
	 * Packet format is preamble + explicit-header + payload + crc
	 * Explicit Header Mode
	 * payload is TO + FROM + ID + FLAGS + message data
	 * RX mode is implmented with RXCONTINUOUS
	 * max message data length is 255 - 4 = 251 octets
	*/

	opmode.bit.mode = SX1276_OPMODE_MODE_STANDBY;
	sx1276_writereg(dev, SX1276_OPMODE, opmode.reg);

	// clear all irq flags
	sx1276_writereg(dev, SX1276_IRQFLAGS, 0x00);

	for(i = 0; i < SX1276_MODEM_CONFIG_LENGTH; i++) {
		sx1276_writereg(dev, SX1276_MODEM_CONFIG + i, modem_config.reg[i]);
	}

	sx1276_set_frequency(dev, 915.0);

	//sx1276_writereg(dev, SX1276_PA_CONFIG, pa_config.reg);
	
	//irqmask.reg = sx1276_readreg(dev, SX1276_IRQFLAGS_MASK);
	//irqmask.bit.rx_done = 1;
	irqmask.reg = 0xff;
	sx1276_writereg(dev, SX1276_IRQFLAGS_MASK, irqmask.reg);

	opmode.bit.mode = SX1276_OPMODE_MODE_RXSINGLE;
	sx1276_writereg(dev, SX1276_OPMODE, opmode.reg);

	return 0;
}

void sx1276_interrupt(void *data) {
	//sx1276_t *dev = (sx1276_t *)data;
	printf("sx1276: RX DONE\r\n");
}

void sx1276_dump_status(sx1276_t *dev) {
	SX1276_OPMODE_Type opmode;
	SX1276_IRQFLAGS_Type flags;
	SX1276_MODEM_STATUS_Type status;
	uint8_t buf[256];
	int rxbytes;
	int i;

	opmode.reg = sx1276_readreg(dev, SX1276_OPMODE);
	printf("sx1276: opmode mode %d\r\n", opmode.bit.mode);

	flags.reg = sx1276_readreg(dev, SX1276_IRQFLAGS);
	printf("        rx_timeout=%d rx_done=%d crc_error=%d valid=%d\r\n        tx_done=%d cad_done=%d fhss=%d cad_detect=%d\r\n",
			flags.bit.rx_timeout,
			flags.bit.rx_done,
			flags.bit.payload_crc_error,
			flags.bit.valid_header,
			flags.bit.tx_done,
			flags.bit.cad_done,
			flags.bit.fhss_change_channel,
			flags.bit.cad_detected);

	status.reg = sx1276_readreg(dev, SX1276_MODEM_STATUS);
	printf("        ");
	if(status.bit.modem_clear) {
		printf("CLEAR ");
	}
	if(status.bit.header_valid) {
		printf("VALID ");
	}
	if(status.bit.rx_ongoing) {
		printf("RX ");
	}
	if(status.bit.sync) {
		printf("SYNC ");
	}
	if(status.bit.detect) {
		printf("DETECT ");
	}
	printf("\r\n");

	rxbytes = sx1276_readfifo(dev, buf, 256);
	if(rxbytes == 0) {
		printf("        fifo empty\r\n");
	}else if(rxbytes < 0) {
		printf("        packet too large\r\n");
	}else{
		printf("        ");
		for(i = 0; i < rxbytes; i++) {
			if(i > 0 && (i % 16) == 0) {
				printf("\r\n        ");
			}
			printf("%02X ", buf[i]);
		}
		printf("\r\n");
	}
}

