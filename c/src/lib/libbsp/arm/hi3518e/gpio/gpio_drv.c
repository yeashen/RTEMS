/******************************************************************************

	Copyright (C), 2015-2025, SHARPNOW Co., Ltd.

 ******************************************************************************
  File Name     : gpio_drv.c
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

#include <hi3518e.h>
#include <gpio_drv.h>

static hi_pinmux_regs_s *hi_pinmux_reg = NULL;
static hi_gpio_regs_s *gpio_reg = NULL;

void hi_gpio_set_derection(GPIO_PIN_NUM pin, GPIO_DIRECTION dir)
{
	hi_pinmux_reg = (hi_pinmux_regs_s *)PINMUX_REG_BASE;
	gpio_reg = (hi_gpio_regs_s *)GPIO9_REG_BASE;
	unsigned int val = gpio_reg->dir;

	hi_pinmux_reg->gpio9_1 = 0x1;

	if(dir == GPIO_OUTPUT){
		gpio_reg->dir = val|(1<<pin);
	}else{
		gpio_reg->dir =val&(~(1<<pin));
	}
}

unsigned int hi_gpio_get(GPIO_PIN_NUM pin)
{
	unsigned int value = 0;

	switch(pin){
		case GPIO_PIN0:
			value = gpio_reg->data0;
			break;
		case GPIO_PIN1:
			value = gpio_reg->data1;
			break;
		case GPIO_PIN2:
			value = gpio_reg->data2;
			break;
		case GPIO_PIN3:
			value = gpio_reg->data3;
			break;
		case GPIO_PIN4:
			value = gpio_reg->data4;
			break;
		case GPIO_PIN5:
			value = gpio_reg->data5;
			break;
		case GPIO_PIN6:
			value = gpio_reg->data6;
			break;
		case GPIO_PIN7:
			value = gpio_reg->data7;
			break;
		default:
			break;
	}
	
	return value;
}

void hi_gpio_set(GPIO_PIN_NUM pin, unsigned int val)
{
	unsigned int value = (val==0)?0:1;

	switch(pin){
		case GPIO_PIN0:
			gpio_reg->data0 = value;
			break;
		case GPIO_PIN1:
			gpio_reg->data1 = value;
			break;
		case GPIO_PIN2:
			gpio_reg->data2 = value;
			break;
		case GPIO_PIN3:
			gpio_reg->data3 = value;
			break;
		case GPIO_PIN4:
			gpio_reg->data4 = value;
			break;
		case GPIO_PIN5:
			gpio_reg->data5 = value;
			break;
		case GPIO_PIN6:
			gpio_reg->data6 = value;
			break;
		case GPIO_PIN7:
			gpio_reg->data7 = value;
			break;
		default:
			break;
	}
}

void hi_gpio_int_config(GPIO_INT_PATA *para)
{
	unsigned int tmp;
	unsigned int pin, type, iev, edge;

	pin = para->pin_int;

	// TODO: pinmux config

	tmp = gpio_reg->is;
	type = para->type;
	edge = para->edge_type;
	if(type == GPIO_EDGE_TRIG){
		gpio_reg->is = tmp&(~(1<<pin));
		tmp = gpio_reg->ibe;
		if(edge == GPIO_SIGGLE_EDGE){
			gpio_reg->ibe = tmp&(~(1<<pin));
		}else{
			gpio_reg->ibe = tmp|(1<<pin);
		}
	}else{
		gpio_reg->is = tmp|(1<<pin);
	}

	tmp =gpio_reg->iev;
	iev = para->iev;
	if((iev == GPIO_FALLING_TRIG) || (iev == GPIO_LOW_TRIG)){
		gpio_reg->iev = tmp&(~(1<<pin));
	}else{
		gpio_reg->iev = tmp|(1<<pin);
	}
	
	gpio_reg->ic = 0xFF;

	tmp = gpio_reg->ie;
	gpio_reg->ie = tmp|(1<<pin);
}

unsigned int hi_gpio_int_query()
{
	return gpio_reg->ris;
}

void hi_gpio_int_clear(GPIO_PIN_NUM pin)
{
	gpio_reg->ic = 1<<pin;
}
