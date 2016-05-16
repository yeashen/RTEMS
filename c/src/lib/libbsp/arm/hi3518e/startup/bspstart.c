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
#include <hi3518e.h>

/*
 *  Show system relation information
 */
static void bsp_show_info(void)
{
	uint32_t i, sysid[4], version = 0;
	
	for(i = 0; i < 4; i++){
		sysid[i] = HI_REG_RD(SYSID_RGE_BASE+i*4);
		version += sysid[i] << (i*8);
	}
	
	printk("\n#################################################\n");
	printk("@	RTEMS Real Time Operating System\n");
	printk("@	System version: V4.11\n");
	printk("@	Cpu version: Hi%x\n", version);
	printk("#################################################\n\n");
}

/*
 *  BSP Specific Initialization in C
 */
void bsp_start(void)
{
	bsp_show_info();
	rtems_exception_init_mngt();
	bsp_interrupt_initialize();
}

