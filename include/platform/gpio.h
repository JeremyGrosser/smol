#ifndef GPIO_H
#define GPIO_H

typedef struct gpio_s gpio_t;
typedef struct pincfg_s pincfg_t;
typedef struct exti_s exti_t;
typedef void (*exti_func_t)(void);

void gpio_setup(gpio_t *gpio);
int gpio_read(gpio_t *gpio);
void gpio_write(gpio_t *gpio, int state);
void gpio_toggle(gpio_t *gpio);

#endif
