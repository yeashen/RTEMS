#ifndef _GPIO_H
#define _GPIO_H

#include <gpio_drv.h>

void gpio_set_derection(GPIO_PIN_NUM pin, GPIO_DIRECTION dir);
unsigned int gpio_get(GPIO_PIN_NUM pin);
void gpio_set(GPIO_PIN_NUM pin, unsigned int val);
void gpio_int_config(GPIO_INT_PATA para);
void gpio_init(GPIO_INT_PATA *para);

#endif /* _GPIO_H */
