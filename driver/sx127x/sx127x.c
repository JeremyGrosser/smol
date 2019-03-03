#include "sx127x.h"

#include <platform.h>
#include <stdbool.h>
#include <stdio.h>

#define SX127X_FXOSC	32000000.0f
#define SX127X_FSTEP	(SX127X_FXOSC / (1 << 19))

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

/*
static void sx127x_modem_config(sx127x_t *dev) {
	RegModemConfig1_t mc1;
	RegModemConfig2_t mc2;
	RegModemConfig3_t mc3;

	mc1.reg = sx127x_readreg(dev, RegModemConfig1);
	mc2.reg = sx127x_readreg(dev, RegModemConfig2);
	mc3.reg = sx127x_readreg(dev, RegModemConfig3);

	printf("SF%u BW%u CR%u\r\n", mc2.bit.SpreadingFactor, mc1.bit.Bw, mc1.bit.CodingRate);
}
*/

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
uint32_t sx127x_get_frequency(sx127x_t *dev) {
	uint32_t frf;

	frf = (sx127x_readreg(dev, RegFrMsb) << 16);
	frf |= (sx127x_readreg(dev, RegFrMid) << 8);
	frf |= sx127x_readreg(dev, RegFrLsb);

	return (frf * SX127X_FSTEP);
}

/* 
 * Set transciever frequency in Hz. Must be in SLEEP mode.
 */
uint32_t sx127x_set_frequency(sx127x_t *dev, uint32_t frequency) {
	uint32_t frf;
	frf = (frequency / SX127X_FSTEP);

	sx127x_writereg(dev, RegFrMsb, ((frf >> 16) & 0xFF));
	sx127x_writereg(dev, RegFrMid, ((frf >> 8) & 0xFF));
	sx127x_writereg(dev, RegFrLsb, frf & 0xFF);

	return sx127x_get_frequency(dev);
}

int sx127x_setup(sx127x_t *dev) {
	RegOpMode_t opmode;
	RegDioMapping1_t diomap;

	dev->errmsg = "No error";

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

	//sx127x_modem_config(dev);
	
	diomap.reg = sx127x_readreg(dev, RegDioMapping1);
	diomap.bit.Dio0Mapping = 0; // RxDone interrupt
	sx127x_writereg(dev, RegDioMapping1, diomap.reg);

	return 0;
}

char *sx127x_get_error(sx127x_t *dev) {
	return dev->errmsg;
}

void sx127x_interrupt(void *data) {
	//sx127x_t *dev = (sx127x_t *)data;
}

uint8_t sx127x_version(sx127x_t *dev) {
	return sx127x_readreg(dev, RegVersion);
}

int sx127x_transmit(sx127x_t *dev, uint8_t *buf, size_t len) {
	RegOpMode_t opmode;
	RegIrqFlags_t irqflags;

	opmode.reg = sx127x_readreg(dev, RegOpMode);
	opmode.bit.Mode = MODE_STANDBY;
	sx127x_writereg(dev, RegOpMode, opmode.reg);

	/* Set FifoAddrPtr to TxBaseAddr */
	sx127x_writereg(dev, RegFifoAddrPtr, sx127x_readreg(dev, RegFifoTxBaseAddr));

	if(spi_transfer(dev->spi, buf, NULL, len) != len) {
		dev->errmsg = "sx127x_transmit: spi_transfer failed";
		return 1;
	}

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

int sx127x_receive(sx127x_t *dev, uint8_t *buf, size_t len) {
	RegOpMode_t opmode;
	RegIrqFlags_t irqflags;
	size_t rxlen;
	int err = 0;
	int i;

	sx127x_writereg(dev, RegFifoAddrPtr, sx127x_readreg(dev, RegFifoRxBaseAddr));

	opmode.reg = sx127x_readreg(dev, RegOpMode);
	opmode.bit.Mode = MODE_RXSINGLE;
	sx127x_writereg(dev, RegOpMode, opmode.reg);

	// Wait for RxDone
	do {
		irqflags.reg = sx127x_readreg(dev, RegIrqFlags);
		//platform_delay(1);
	}while(!(irqflags.bit.RxDone | irqflags.bit.RxTimeout));

	if(irqflags.bit.RxTimeout) {
		dev->errmsg = "RX Timeout";
		err = -1;
		goto rxerr;
	}

	if(irqflags.bit.PayloadCrcError) {
		dev->errmsg = "RX Payload CRC Error";
		err = -2;
		goto rxerr;
	}

	if(!irqflags.bit.ValidHeader) {
		dev->errmsg = "RX Invalid Header";
		err = -3;
		goto rxerr;
	}

	rxlen = sx127x_readreg(dev, RegRxNbBytes);
	if(rxlen > len) {
		dev->errmsg = "Received packet larger than buffer";
		err = -4;
		goto rxerr;
	}

	sx127x_writereg(dev, RegFifoAddrPtr, sx127x_readreg(dev, RegFifoRxCurrentAddr));
	for(i = 0; i < rxlen; i++) {
		buf[i] = sx127x_readreg(dev, RegFifo);
	}
	err = rxlen;

rxerr:
	// Clear all IRQ flags
	sx127x_writereg(dev, RegIrqFlags, 0xFF);

	opmode.bit.Mode = MODE_SLEEP;
	sx127x_writereg(dev, RegOpMode, opmode.reg);
	return err;
}
