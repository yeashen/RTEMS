/******************************************************************************

	Copyright (C), 2015-2025, SHARPNOW Co., Ltd.

 ******************************************************************************
  File Name     : bspstart.c
  Version       : Initial Draft
  Author        : Juven
  Created       : 2016/03/16
  Last Modified : 
  Description   : 
  History       : 
  1.Date        : 2016/03/16
   Author       : Juven
   Modification : Created file

******************************************************************************/
#include <bsp.h>
#include <bsp/irq-generic.h>
#include <libchip/virreg_hal.h>

#include <hi3518e.h>

/*
  *  show chip information
  */
 void bsp_show_info()
{
	unsigned int i, id, chipid[4];

	for(i = 0; i< 4; i++){
		chipid[i] = HI_REG_RD(0x20050EE0+i*4)&0xFF;
	}
	id = chipid[0] | (chipid[1]<<8) | (chipid[2]<<16) | (chipid[3]<<24);
	
	printk("\n****************************************************\n");
	printk("* RTEMS Real Time Operating System (RTOS) \n");
	printk("* Chip: %x\n", id);
	printk("* Build Date: %s - %s\n", __DATE__, __TIME__);
	printk("****************************************************\n\n");
}

/*
 *  BSP Specific Initialization in C
 */
void bsp_start(void)
{
	rtems_exception_init_mngt();
	bsp_interrupt_initialize();
	bsp_show_info();
}

