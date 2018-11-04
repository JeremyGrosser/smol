#ifndef INA3221_H
#define INA3221_H
#include <platform/i2c.h>

#define INA3221_REG_CONFIG				0x00
#define INA3221_REG_CH1_SHUNT_VOLTS		0x01
#define INA3221_REG_CH1_BUS_VOLTS		0x02
#define INA3221_REG_CH2_SHUNT_VOLTS		0x03
#define INA3221_REG_CH2_BUS_VOLTS		0x04
#define INA3221_REG_CH3_SHUNT_VOLTS		0x05
#define INA3221_REG_CH3_BUS_VOLTS		0x06
#define INA3221_REG_CH1_CRIT_LIMIT		0x07
#define INA3221_REG_CH1_WARN_LIMIT		0x08
#define INA3221_REG_CH2_CRIT_LIMIT		0x09
#define INA3221_REG_CH2_WARN_LIMIT		0x0A
#define INA3221_REG_CH3_CRIT_LIMIT		0x0B
#define INA3221_REG_CH3_WARN_LIMIT		0x0C
#define INA3221_REG_SHUNT_VOLTS_SUM		0x0D
#define INA3221_REG_SHUNT_VOLTS_LIMIT	0x0E
#define INA3221_REG_MASK_ENABLE			0x0F
#define INA3221_REG_PV_UPPER_LIMIT		0x10
#define INA3221_REG_PV_LOWER_LIMIT		0x11
#define INA3221_REG_MFG_ID				0xFE
#define INA3221_REG_DIE_ID				0xFF

struct ina3221_s {
	i2c_t *i2c;
	uint8_t address;
};
typedef struct ina3221_s ina3221_t;

int ina3221_setup(ina3221_t *dev);
uint16_t ina3221_read(ina3221_t *dev, uint8_t reg);
int ina3221_write(ina3221_t *dev, uint8_t reg, uint16_t val);

#endif
