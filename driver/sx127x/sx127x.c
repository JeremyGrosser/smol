#include "sx127x.h"

#include <platform.h>
#include <stdbool.h>
#include <stdio.h>

#define SX127X_FXOSC	32000000.0f
//#define SX127X_FSTEP	(SX127X_FXOSC / (1 << 19))
#define SX127X_FSTEP	61

enum sx127x_errno {
	ERR_FREQUENCY,
	ERR_OPMODE,
};

enum sx127x_register {
	RegFifo = 0x00,
	RegOpMode = 0x01,
	RegFrMsb = 0x06,
	RegFrMid = 0x07,
	RegFrLsb = 0x08,
	RegVersion = 0x42,
	RegFifoAddrPtr = 0x0D,
	RegFifoTxBaseAddr = 0x0E,
	RegFifoRxBaseAddr = 0x0F,
	RegFifoRxCurrentAddr = 0x10,
	RegFifoRxByteAddr = 0x25,
	RegRxNbBytes = 0x13,
	RegIrqFlags = 0x12,
	RegModemStat = 0x18,
	RegModemConfig1 = 0x1D,
	RegModemConfig2 = 0x1E,
	RegModemConfig3 = 0x26,
	RegDioMapping1 = 0x40,
	RegDioMapping2 = 0x41,
	RegSyncWord = 0x39,
	RegPktSnrValue = 0x19,
	RegPktRssiValue = 0x1A,
};

typedef union {
	struct {
		enum {
			MODE_SLEEP,
			MODE_STANDBY,
			MODE_FSTX,
			MODE_TX,
			MODE_FSRX,
			MODE_RXCONTINUOUS,
			MODE_RXSINGLE,
			MODE_CAD,
		} Mode:3;
		bool LowFrequencyModeOn:1;
		uint8_t :2; // reserved
		bool AccessSharedReg:1;
		bool LongRangeMode:1;
	} bit;
	uint8_t reg;
} __attribute__((packed)) RegOpMode_t;

typedef union {
	struct {
		bool CadDetected:1;
		bool FhssChangeChannel:1;
		bool CadDone:1;
		bool TxDone:1;
		bool ValidHeader:1;
		bool PayloadCrcError:1;
		bool RxDone:1;
		bool RxTimeout:1;
	} bit;
	uint8_t reg;
} __attribute__((packed)) RegIrqFlags_t;

typedef union {
	struct {
		bool SignalDetected:1;
		bool SignalSync:1;
		bool RxOngoing:1;
		bool HeaderValid:1;
		bool ModemClear:1;
		uint8_t RxCodingRate:3;
	} bit;
	uint8_t reg;
} __attribute__((packed)) RegModemStat_t;

typedef union {
	struct {
		bool ImplicitHeaderModeOn:1;
		uint8_t CodingRate:3;
		uint8_t Bw:4;
	} bit;
	uint8_t reg;
} __attribute__((packed)) RegModemConfig1_t;

typedef union {
	struct {
		uint8_t SymbTimeoutMsb:2;
		bool RxPayloadCrcOn:1;
		bool TxContinuousMode:1;
		uint8_t SpreadingFactor:4;
	} bit;
	uint8_t reg;
} __attribute__((packed)) RegModemConfig2_t;

typedef union {
	struct {
		uint8_t reserved:2;
		bool AgcAutoOn:1;
		bool LowDataRateOptimize:1;
		uint8_t unused:4;
	} bit;
	uint8_t reg;
} __attribute__((packed)) RegModemConfig3_t;

typedef union {
	struct {
		uint8_t Dio3Mapping:2;
		uint8_t Dio2Mapping:2;
		uint8_t Dio1Mapping:2;
		uint8_t Dio0Mapping:2;
	} bit;
	uint8_t reg;
} __attribute__((packed)) RegDioMapping1_t;

typedef union {
	struct {
		bool MapPreambleDetect:1;
		uint8_t reserved:3;
		uint8_t Dio5Mapping:2;
		uint8_t Dio4Mapping:2;
	} bit;
	uint8_t reg;
} __attribute__((packed)) RegDioMapping2_t;

static uint8_t sx127x_readreg(sx127x_t *dev, enum sx127x_register reg) {
	uint8_t out[2];
	uint8_t in[2];

	out[0] = reg & 0x7f;
	out[1] = 0x00;

	spi_transfer(dev->spi, out, in, 2);
	return in[1];
}

static uint8_t sx127x_writereg(sx127x_t *dev, enum sx127x_register reg, uint8_t val) {
	uint8_t out[2];
	uint8_t in[2];

	out[0] = reg | 0x80;
	out[1] = val;

	spi_transfer(dev->spi, out, in, 2);
	return in[1];
}

static void sx127x_reset(sx127x_t *dev) {
	gpio_setup(dev->reset);
	gpio_setup(dev->dio0);
	spi_setup(dev->spi);

	gpio_write(dev->reset, 0);
	platform_delay(1);
	gpio_write(dev->reset, 1);
	/* 
	 * The datasheet only specifies 5ms wait after reset, but we get odd
	 * RegOpMode values if we don't wait a bit longer.
	 */
	platform_delay(100); 
}

void sx127x_modem_config(sx127x_t *dev, uint8_t sf, uint8_t bw, uint8_t cr, uint8_t syncword) {
	RegModemConfig1_t mc1;
	RegModemConfig2_t mc2;
	RegModemConfig3_t mc3;

	mc1.reg = sx127x_readreg(dev, RegModemConfig1);
	mc2.reg = sx127x_readreg(dev, RegModemConfig2);
	mc3.reg = sx127x_readreg(dev, RegModemConfig3);

	mc1.bit.Bw = bw;
	mc1.bit.CodingRate = cr;
	mc2.bit.SpreadingFactor = sf;

	sx127x_writereg(dev, RegModemConfig1, mc1.reg);
	sx127x_writereg(dev, RegModemConfig2, mc2.reg);
	sx127x_writereg(dev, RegModemConfig3, mc3.reg);
	sx127x_writereg(dev, RegSyncWord, syncword);
}

/*
static void sx127x_modem_status(sx127x_t *dev) {
	RegModemStat_t status;

	status.reg = sx127x_readreg(dev, RegModemStat);
	if(status.bit.SignalDetected) {
		printf("DETECT ");
	}
	if(status.bit.SignalSync) {
		printf("SYNC ");
	}
	if(status.bit.RxOngoing) {
		printf("RX ");
	}
	if(status.bit.HeaderValid) {
		printf("HDR ");
	}
	if(status.bit.ModemClear) {
		printf("CLR ");
	}
	printf("\r\n");
}
*/

/* Get transceiver frequency in Hz */
static uint32_t sx127x_get_frequency(sx127x_t *dev) {
	uint32_t frf;

	frf = (sx127x_readreg(dev, RegFrMsb) << 16);
	frf |= (sx127x_readreg(dev, RegFrMid) << 8);
	frf |= sx127x_readreg(dev, RegFrLsb);

	return (frf * SX127X_FSTEP);
}

/* 
 * Set transciever frequency in Hz. Must be in SLEEP mode.
 */
static uint32_t sx127x_set_frequency(sx127x_t *dev, uint32_t frequency) {
	uint32_t frf;
	frf = (frequency / SX127X_FSTEP);

	sx127x_writereg(dev, RegFrMsb, ((frf >> 16) & 0xFF));
	sx127x_writereg(dev, RegFrMid, ((frf >> 8) & 0xFF));
	sx127x_writereg(dev, RegFrLsb, frf & 0xFF);

	return sx127x_get_frequency(dev);
}

int sx127x_set_channel(sx127x_t *dev, lorawan_channel_t *channels, lorawan_data_rate_t *data_rates, uint8_t chan_num, uint8_t dr_num) {
	lorawan_channel_t chan;
	lorawan_data_rate_t dr;
	RegOpMode_t opmode;

	opmode.reg = sx127x_readreg(dev, RegOpMode);
	if(opmode.bit.Mode > MODE_STANDBY) {
		dev->errmsg = "Cannot change channel outside of SLEEP and STANDBY modes";
		return -1;
	}

	chan = channels[chan_num];
	if(dr_num < chan.min_dr || dr_num > chan.max_dr) {
		dev->errmsg = "Invalid data rate for channel";
		return -2;
	}
	dr = data_rates[dr_num];

	sx127x_set_frequency(dev, chan.freq);
	sx127x_modem_config(dev, dr.sf, dr.bw, chan.cr, LORAWAN_SYNC_WORD);

	printf("CHAN:%2u DR:%2u (SF:%2u BW:%2u CR:%2u FRF:%ld)\r\n",
			chan_num, dr_num, dr.sf, dr.bw, chan.cr, chan.freq);

	return 0;
}

int sx127x_setup(sx127x_t *dev) {
	RegOpMode_t opmode;
	RegDioMapping1_t diomap;

	dev->errmsg = "No error";
	dev->rx_done = false;
	dev->count_packets_rx = 0;
	dev->count_errors_rx = 0;

	sx127x_reset(dev);

	opmode.reg = sx127x_readreg(dev, RegOpMode);
	opmode.bit.Mode = MODE_SLEEP;
	sx127x_writereg(dev, RegOpMode, opmode.reg);

	opmode.bit.LongRangeMode = true;
	opmode.bit.AccessSharedReg = false;
	sx127x_writereg(dev, RegOpMode, opmode.reg);

	if(sx127x_readreg(dev, RegOpMode) != opmode.reg) {
		dev->errmsg = "Setting RegOpMode failed";
		return ERR_OPMODE;
	}

	sx127x_writereg(dev, RegFifoRxBaseAddr, 0);
	sx127x_writereg(dev, RegRxNbBytes, 64);
	sx127x_writereg(dev, RegFifoTxBaseAddr, 64);
	sx127x_writereg(dev, RegFifoAddrPtr, 0);

	diomap.reg = sx127x_readreg(dev, RegDioMapping1);
	diomap.bit.Dio0Mapping = 0; // RxDone interrupt
	sx127x_writereg(dev, RegDioMapping1, diomap.reg);

	return 0;
}

char *sx127x_get_error(sx127x_t *dev) {
	return dev->errmsg;
}

void sx127x_interrupt(void *data) {
	sx127x_t *dev = (sx127x_t *)data;
	RegIrqFlags_t irqflags;

	irqflags.reg = sx127x_readreg(dev, RegIrqFlags);
	if(irqflags.bit.RxDone) {
		dev->rx_done = true;
	}
}

uint8_t sx127x_version(sx127x_t *dev) {
	return sx127x_readreg(dev, RegVersion);
}

int sx127x_transmit(sx127x_t *dev, uint8_t *buf, size_t len) {
	RegOpMode_t opmode;
	RegIrqFlags_t irqflags;
	int i;

	opmode.reg = sx127x_readreg(dev, RegOpMode);
	opmode.bit.Mode = MODE_STANDBY;
	sx127x_writereg(dev, RegOpMode, opmode.reg);

	/* Set FifoAddrPtr to TxBaseAddr */
	sx127x_writereg(dev, RegFifoAddrPtr, sx127x_readreg(dev, RegFifoTxBaseAddr));

	for(i = 0; i < len; i++) {
		sx127x_writereg(dev, RegFifo, buf[i]);
	}

	/*
	if(spi_transfer(dev->spi, buf, NULL, len) != len) {
		dev->errmsg = "sx127x_transmit: spi_transfer failed";
		return 1;
	}
	*/

	opmode.bit.Mode = MODE_TX;
	sx127x_writereg(dev, RegOpMode, opmode.reg);

	// Wait for TxDone
	do {
		irqflags.reg = sx127x_readreg(dev, RegIrqFlags);
		platform_delay(1);
	}while(!irqflags.bit.TxDone);

	// Clear the TxDone flag
	irqflags.reg = 0x00;
	irqflags.bit.TxDone = true;
	sx127x_writereg(dev, RegIrqFlags, irqflags.reg);

	opmode.bit.Mode = MODE_SLEEP;
	sx127x_writereg(dev, RegOpMode, opmode.reg);

	return 0;
}

int sx127x_receive(sx127x_t *dev, uint8_t *buf, size_t len, uint32_t timeout_ms) {
	RegOpMode_t opmode;
	RegIrqFlags_t irqflags;
	uint32_t now;
	size_t rxlen;
	int err = 0;
	int i;

	opmode.reg = sx127x_readreg(dev, RegOpMode);
	if(opmode.bit.Mode != MODE_RXSINGLE) {
		opmode.bit.Mode = MODE_RXSINGLE;
		sx127x_writereg(dev, RegOpMode, opmode.reg);
		sx127x_writereg(dev, RegFifoAddrPtr, sx127x_readreg(dev, RegFifoRxBaseAddr));
	}

	// Wait for RxDone
	now = platform_ticks();
	while(!dev->rx_done && (platform_ticks() < (now + timeout_ms))) {
		__WFI();
	}

	if(!dev->rx_done) {
		dev->errmsg = "RX interrupt timeout";
		err = -1;
		goto rxend;
	}

	dev->rx_done = false;

	irqflags.reg = sx127x_readreg(dev, RegIrqFlags);

	if(irqflags.bit.RxTimeout) {
		dev->errmsg = "RF RX timeout";
		err = -1;
		goto rxend;
	}

	if(irqflags.bit.PayloadCrcError) {
		dev->errmsg = "RX Payload CRC Error";
		err = -3;
		goto rxerr;
	}

	if(!irqflags.bit.ValidHeader) {
		dev->errmsg = "RX Invalid Header";
		err = -4;
		goto rxerr;
	}

	rxlen = sx127x_readreg(dev, RegRxNbBytes);
	if(rxlen > len) {
		dev->errmsg = "Received packet larger than buffer";
		err = -5;
		goto rxerr;
	}
	if(rxlen == 0) {
		dev->errmsg = "Received empty packet";
		err = -6;
		goto rxerr;
	}

	sx127x_writereg(dev, RegFifoAddrPtr, sx127x_readreg(dev, RegFifoRxCurrentAddr));
	for(i = 0; i < rxlen; i++) {
		buf[i] = sx127x_readreg(dev, RegFifo);
	}
	err = rxlen;
	dev->count_packets_rx++;

	printf("SNR: %u RSSI: %d\r\n",
			sx127x_readreg(dev, RegPktSnrValue),
			((int)sx127x_readreg(dev, RegPktRssiValue) - 157));

	goto rxend;

rxerr:
	dev->count_errors_rx++;

rxend:
	// Clear all IRQ flags
	sx127x_writereg(dev, RegIrqFlags, 0xFF);

	// Go to sleep unless we're in an async receive waiting for RxDone interrupt
	if(timeout_ms != 0) {
		opmode.bit.Mode = MODE_SLEEP;
		sx127x_writereg(dev, RegOpMode, opmode.reg);
	}

	return err;
}
