#ifndef SX127X_INTERNAL_H
#define SX127X_INTERNAL_H

#include <stdint.h>
#include <stdbool.h>

#define SX127X_FSOSC 32000000.0f
//#define SX127X_FSTEP (SX127x_FXOSC / (1 << 19))
#define SX127X_FSTEP 61

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


uint8_t sx127x_readreg(sx127x_t *dev, enum sx127x_register reg);
uint8_t sx127x_writereg(sx127x_t *dev, enum sx127x_register reg, uint8_t val);
uint32_t sx127x_get_frequency(sx127x_t *dev);
uint32_t sx127x_set_frequency(sx127x_t *dev, uint32_t frequency);

#endif // SX127X_INTERNAL_H
