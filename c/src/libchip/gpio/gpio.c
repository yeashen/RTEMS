/******************************************************************************

	Copyright (C), 2015-2025, SHARPNOW Co., Ltd.

 ******************************************************************************
  File Name     : gpio.c
  Version       : Initial Draft
  Author        : Juven
  Created       : 2016/04/26
  Last Modified : 
  Description   : 
  History       : 
  1.Date        : 2016/04/26
   Author       : Juven
   Modification : Created file

******************************************************************************/
#include <rtems.h>
#include <libchip/gpio.h>
#include <bsp/irq.h>
#include <platform.h>

static rtems_isr gpio_isr(rtems_vector_number vector)
{
	unsigned int int_sta;

	int_sta = hi_gpio_int_query();
	if(int_sta & (1<<GPIO_PIN0)){
		printk("gpio0 interrupt\n");
		bsp_interrupt_vector_clear(vector);
		hi_gpio_int_clear(GPIO_PIN0);
	}
}

void gpio_set_derection(GPIO_PIN_NUM pin, GPIO_DIRECTION dir)
{
	hi_gpio_set_derection(pin, dir);
}

unsigned int gpio_get(GPIO_PIN_NUM pin)
{
	return hi_gpio_get(pin);
}

void gpio_set(GPIO_PIN_NUM pin, unsigned int val)
{
	hi_gpio_set(pin, val);
}

void gpio_init(GPIO_INT_PATA *para)
{
	int status;
	unsigned int int_src;
	unsigned int pin;

	pin = para->pin_int;
	switch(pin)
	{
		case GPIO_PIN0:
		case GPIO_PIN1:
		case GPIO_PIN2:
			int_src = BSP_INT_GPIO_0_1_2_11;
			break;
		case GPIO_PIN3:
		case GPIO_PIN4:
		case GPIO_PIN5:
			int_src = BSP_INT_GPIO_3_4_5_10;
			break;
		case GPIO_PIN6:
		case GPIO_PIN7:
			int_src = BSP_INT_GPIO_6_7_8_9;
			break;
		default:
			int_src = BSP_INT_GPIO_0_1_2_11;
			break;
	}

	status = rtems_interrupt_handler_install(
			int_src,
			"GPIO",
			RTEMS_INTERRUPT_UNIQUE,
			(rtems_interrupt_handler) gpio_isr,
			NULL
			);
	if(status != RTEMS_SUCCESSFUL){
		printk("gpio install interrupt handler failed!\n");
		return -1;
	}

	hi_gpio_int_config(para);
}
