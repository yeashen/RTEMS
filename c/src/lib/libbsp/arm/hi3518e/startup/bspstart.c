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
 *  BSP Specific Initialization in C
 */
void bsp_start(void)
{
	rtems_exception_init_mngt();
	bsp_interrupt_initialize();
}

