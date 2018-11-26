#include <platform/spi.h>
#include <platform.h>
#include "sx1231.h"

#include <stdio.h>


#define REG_FIFO		0x00	/* FIFO read/write access */
#define REG_OP_MODE		0x01	/* Operating modes of the transceiver */
#define REG_DATA_MODUL	0x02	/* Data operation mode and Modulation settings */
#define REG_BITRATE_MSB	0x03	/* Bit Rate setting, Most Significant Bits */
#define REG_BITRATE_LSB	0x04	/* Bit Rate setting, Least Significant Bits */
#define REG_FDEV_MSB	0x05	/* Frequency Deviation setting, Most Significant Bits */
#define REG_FDEV_LSB	0x06	/* Frequency Deviation setting, Least Significant Bits */
#define REG_FRF_MSB		0x07	/* RF Carrier Frequency, Most Significant Bits */
#define REG_FRF_MID		0x08	/* RF Carrier Frequency, Intermediate Bits */
#define REG_FRF_LSB		0x09	/* RF Carrier Frequency, Least Significant Bits */
#define REG_OSC1		0x0A	/* RC Oscillators Settings */
#define REG_AFC_CTRL	0x0B	/* AFC control in low modulation index situations */
#define REG_LOW_BAT		0x0C	/* Low Battery Indicator Settings */
#define REG_LISTEN1		0x0D	/* Listen Mode settings */
#define REG_LISTEN2		0x0E	/* Listen Mode Idle duration */
#define REG_LISTEN3		0x0F	/* Listen Mode Rx duration */
#define REG_VERSION		0x10	/* Semtech ID relating the silicon revision */
/*
0x10 RegVersion 0x23 
0x11 RegPaLevel 0x9F PA selection and Output Power control
0x12 RegPaRamp 0x09 Control of the PA ramp time in FSK mode
0x13 RegOcp 0x1A Over Current Protection control
0x14 Reserved14 0x40 -
0x15 Reserved15 0xB0 -
0x16 Reserved16 0x7B -
0x17 Reserved17 0x9B -
0x18 RegLna 0x08 0x88 LNA settings
0x19 RegRxBw 0x86 0x55 Channel Filter BW Control
0x1A RegAfcBw 0x8A 0x8B Channel Filter BW control during the AFC routine
0x1B RegOokPeak 0x40 OOK demodulator selection and control in peak mode
0x1C RegOokAvg 0x80 Average threshold control of the OOK demodulator
0x1D RegOokFix 0x06 Fixed threshold control of the OOK demodulator
0x1E RegAfcFei 0x10 AFC and FEI control and status
0x1F RegAfcMsb 0x00 MSB of the frequency correction of the AFC
0x20 RegAfcLsb 0x00 LSB of the frequency correction of the AFC
0x21 RegFeiMsb 0x00 MSB of the calculated frequency error
0x22 RegFeiLsb 0x00 LSB of the calculated frequency error
0x23 RegRssiConfig 0x02 RSSI-related settings
0x24 RegRssiValue 0xFF RSSI value in dBm
0x25 RegDioMapping1 0x00 Mapping of pins DIO0 to DIO3
0x26 RegDioMapping2 0x05 0x07 Mapping of pins DIO4 and DIO5, ClkOut frequency
0x27 RegIrqFlags1 0x80 Status register: PLL Lock state, Timeout, RSSI > Threshold...
0x28 RegIrqFlags2 0x00 Status register: FIFO handling flags, Low Battery detection...
0x29 RegRssiThresh 0xFF 0xE4 RSSI Threshold control
0x2A RegRxTimeout1 0x00 Timeout duration between Rx request and RSSI detection
0x2B RegRxTimeout2 0x00 Timeout duration between RSSI detection and PayloadReady
0x2C RegPreambleMsb 0x00 Preamble length, MSB
0x2D RegPreambleLsb 0x03 Preamble length, LSB
0x2E RegSyncConfig 0x98 Sync Word Recognition control
0x2F-0x36 RegSyncValue1-8 0x00 0x01 Sync Word bytes, 1 through 8
0x37 RegPacketConfig1 0x10 Packet mode settings
0x38 RegPayloadLength 0x40 Payload length setting
0x39 RegNodeAdrs 0x00 Node address
0x3A RegBroadcastAdrs 0x00 Broadcast address
0x3B RegAutoModes 0x00 Auto modes settings
0x3C RegFifoThresh 0x0F 0x8F Fifo threshold, Tx start condition
0x3D RegPacketConfig2 0x02 Packet mode settings
0x3E-0x4D RegAesKey1-16 0x00 16 bytes of the cypher key
0x4E RegTemp1 0x01 Temperature Sensor control
0x4F RegTemp2 0x00 Temperature readout
0x58 RegTestLna 0x1B Sensitivity boost
0x59 RegTestTcxo 0x09 XTAL or TCXO input selection
0x5F RegTestllBw 0x08 PLL Bandwidth setting
0x6F RegTestDagc 0x00 0x30 Fading Margin Improvement
0x71 RegTestAfc 0x00 AFC offset for low modulation index AFC
0x50 + RegTest - Internal test registers
*/


int sx1231_setup(sx1231_t *dev) {
	//uint8_t version;

	gpio_setup(dev->reset);
	gpio_write(dev->reset, 1);
	spi_setup(dev->spi);

	platform_delay(100);
	gpio_write(dev->reset, 0);
	platform_delay(5);

	/*
	version = sx1231_readreg(dev, REG_VERSION);

	printf("sx1231: version 0x%02X\n", version);
	*/

	return 0;
}
