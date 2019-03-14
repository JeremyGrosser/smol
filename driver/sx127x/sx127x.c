#include "sx127x.h"
#include "sx127x_internal.h"

#include <platform/gpio.h>
#include <platform.h>
#include <stdbool.h>
#include <stdio.h>

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
		platform_wait();
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
