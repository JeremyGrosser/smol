#include <platform/i2c.h>
#include <platform.h>

void i2c_setup(i2c_t *i2c) {
    i2c->twi = ((NRF_TWI_Type *)(NRF_TWI0_BASE + (0x1000 * i2c->num)));
    gpio_setup(i2c->scl);
    gpio_setup(i2c->sda);

    i2c->twi->POWER = (TWI_POWER_POWER_Enabled << TWI_POWER_POWER_Pos);
    i2c->twi->PSELSCL = i2c->scl->num;
    i2c->twi->PSELSDA = i2c->sda->num;

    NVIC_EnableIRQ(SPI0_TWI0_IRQn + i2c->num);
}

int i2c_read(i2c_t *i2c, uint8_t address, uint8_t *data, size_t len) {
    size_t i;

    i2c->twi->ADDRESS = (address << 1) | 1;
    i2c->twi->TASKS_STARTRX = 1;

    for(i = 0; i < len; i++) {
        while(!i2c->twi->EVENTS_RXDREADY) {
            __WFI();
        }
        i2c->twi->EVENTS_RXDREADY = 0;
        data[i] = i2c->twi->RXD;
        if(i2c->twi->EVENTS_STOPPED) {
            i2c->twi->EVENTS_STOPPED = 0;
            break;
        }
        i2c->twi->TASKS_RESUME = 1;
    }

	return (int)i;
}

int i2c_write(i2c_t *i2c, uint8_t address, uint8_t *data, size_t len) {
    size_t i;

    i2c->twi->ADDRESS = (address << 1);
    i2c->twi->TASKS_STARTTX = 1;

    for(i = 0; i < len; i++) {
        i2c->twi->TXD = data[i];
        while(!i2c->twi->EVENTS_TXDSENT) {
            __WFI();
        }
        i2c->twi->EVENTS_TXDSENT = 0;
    }
    i2c->twi->TASKS_STOP = 1;

	return (int)i;
}

void SPI0_TWI0_IRQHandler(void) {
}

void SPI1_TWI1_IRQHandler(void) {
}
