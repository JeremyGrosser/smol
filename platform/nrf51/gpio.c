#include <platform/gpio.h>
#include <platform.h>
#include <board.h>

#include <stddef.h>

static exti_func_t exti_functions[1];
static int exti_initialized = 0;

static void exti_init(void) {
    // TODO
}

static void exti_attach(exti_t *exti) {
}

void gpio_setup(gpio_t *gpio) {
    uint32_t pincnf = 0;

    gpio->mask = (1 << gpio->num);

	if(gpio->config.direction == DIR_OUT) {
        pincnf |= (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos) | \
                  (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos);
        NRF_GPIO->DIRSET |= gpio->mask;
	}else{
        pincnf |= (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos) | \
                  (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos);
        NRF_GPIO->DIRCLR |= gpio->mask;
	}

	switch(gpio->config.pull) {
        case PULL_DISABLE:
            pincnf |= (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos);
            break;
        case PULL_ENABLE:
            pincnf |= (GPIO_PIN_CNF_PULL_Pullup << GPIO_PIN_CNF_PULL_Pos) | \
                      (GPIO_PIN_CNF_PULL_Pulldown << GPIO_PIN_CNF_PULL_Pos);
            break;
        case PULL_UP:
            pincnf |= (GPIO_PIN_CNF_PULL_Pullup << GPIO_PIN_CNF_PULL_Pos);
            break;
        case PULL_DOWN:
            pincnf |= (GPIO_PIN_CNF_PULL_Pulldown << GPIO_PIN_CNF_PULL_Pos);
            break;
        default:
            break;
    }

	switch(gpio->config.drive) {
        case DRIVE_HIGH:
            pincnf |= (GPIO_PIN_CNF_DRIVE_H0H1 << GPIO_PIN_CNF_DRIVE_Pos);
            break;
        case DRIVE_LOW:
            pincnf |= (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos);
            break;
        case DRIVE_DISCONNECT:
            pincnf |= (GPIO_PIN_CNF_DRIVE_S0D1 << GPIO_PIN_CNF_DRIVE_Pos);
            break;
        default:
            break;
	}

    if(gpio->interrupt.function != NULL) {
        pincnf |= (gpio->interrupt.sense << GPIO_PIN_CNF_SENSE_Pos) & GPIO_PIN_CNF_SENSE_Msk;
		exti_attach(&gpio->interrupt);
    }

    NRF_GPIO->PIN_CNF[gpio->num] = pincnf;
}

int gpio_read(gpio_t *gpio) {
	if(gpio->config.direction == DIR_IN) {
		return (NRF_GPIO->IN & gpio->mask) ? 1 : 0;
	}else{
		return (NRF_GPIO->OUT & gpio->mask) ? 1 : 0;
	}
}

void gpio_write(gpio_t *gpio, int state) {
	if(state == 0) {
		NRF_GPIO->OUTCLR = gpio->mask;
	}else{
		NRF_GPIO->OUTSET = gpio->mask;
	}
}

void gpio_toggle(gpio_t *gpio) {
    gpio_write(gpio, !gpio_read(gpio));
}
