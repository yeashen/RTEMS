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

void hi_gpio_set_derection(GPIO_PIN_NUM pin, GPIO_DIRECTION dir)
{
	unsigned int val = GPIO_RD_REG(GPIO_DIR);

	if(dir == GPIO_OUTPUT){
		GPIO_WR_REG(GPIO_DIR, val|(1<<pin));
	}else{
		GPIO_WR_REG(GPIO_DIR, val&(~(1<<pin)));
	}
}

unsigned int hi_gpio_get(GPIO_PIN_NUM pin)
{
	unsigned int offset = 0;

	switch(pin){
		case GPIO_PIN0:
			offset = GPIO_0_DATA;
			break;
		case GPIO_PIN1:
			offset = GPIO_1_DATA;
			break;
		case GPIO_PIN2:
			offset = GPIO_2_DATA;
			break;
		case GPIO_PIN3:
			offset = GPIO_3_DATA;
			break;
		case GPIO_PIN4:
			offset = GPIO_5_DATA;
			break;
		case GPIO_PIN5:
			offset = GPIO_5_DATA;
			break;
		case GPIO_PIN6:
			offset = GPIO_6_DATA;
			break;
		case GPIO_PIN7:
			offset = GPIO_7_DATA;
			break;
		default:
			break;
	}
	
	return GPIO_RD_REG(offset);
}

void hi_gpio_set(GPIO_PIN_NUM pin, unsigned int val)
{
	unsigned int offset = 0;

	switch(pin){
		case GPIO_PIN0:
			offset = GPIO_0_DATA;
			break;
		case GPIO_PIN1:
			offset = GPIO_1_DATA;
			break;
		case GPIO_PIN2:
			offset = GPIO_2_DATA;
			break;
		case GPIO_PIN3:
			offset = GPIO_3_DATA;
			break;
		case GPIO_PIN4:
			offset = GPIO_5_DATA;
			break;
		case GPIO_PIN5:
			offset = GPIO_5_DATA;
			break;
		case GPIO_PIN6:
			offset = GPIO_6_DATA;
			break;
		case GPIO_PIN7:
			offset = GPIO_7_DATA;
			break;
		default:
			break;
	}

	GPIO_WR_REG(offset, (val==0)?0:1);
}

void hi_gpio_int_config(GPIO_INT_PATA *para)
{
	unsigned int tmp;
	unsigned int pin, type, iev, edge;

	pin = para->pin_int;

	MUXPIN_WR_REG(GPIO9_0_PIN+(pin*4), 0x1);

	tmp = GPIO_RD_REG(GPIO_IS);
	type = para->type;
	edge = para->edge_type;
	if(type == GPIO_EDGE_TRIG){
		GPIO_WR_REG(GPIO_IS, tmp&(~(1<<pin)));
		tmp = GPIO_RD_REG(GPIO_IBE);
		if(edge == GPIO_SIGGLE_EDGE){
			GPIO_WR_REG(GPIO_IBE, tmp&(~(1<<pin)));
		}else{
			GPIO_WR_REG(GPIO_IBE, tmp|(1<<pin));
		}
	}else{
		GPIO_WR_REG(GPIO_IS, tmp|(1<<pin));
	}

	tmp = GPIO_RD_REG(GPIO_IEV);
	iev = para->iev;
	if((iev == GPIO_FALLING_TRIG) || (iev == GPIO_LOW_TRIG)){
		GPIO_WR_REG(GPIO_IEV, tmp&(~(1<<pin)));
	}else{
		GPIO_WR_REG(GPIO_IEV, tmp|(1<<pin));
	}
	
	GPIO_WR_REG(GPIO_IC, 0xFF);

	tmp = GPIO_RD_REG(GPIO_IE);
	GPIO_WR_REG(GPIO_IE, tmp|(1<<pin));
}

unsigned int hi_gpio_int_query()
{
	return GPIO_RD_REG(GPIO_RIS);
}

void hi_gpio_int_clear(GPIO_PIN_NUM pin)
{
	GPIO_WR_REG(GPIO_IC, 1<<pin);
}
