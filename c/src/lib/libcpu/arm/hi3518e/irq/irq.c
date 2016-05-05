/******************************************************************************

	Copyright (C), 2015-2025, SHARPNOW Co., Ltd.

 ******************************************************************************
  File Name     : irq.c
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
#include <bsp/irq.h>
#include <bsp/irq-generic.h>
#include <rtems/score/armv4.h>

#include <platform.h>

void bsp_interrupt_dispatch(void)
{
	uint32_t regval = INT_RD_REG(REG_INTC_IRQSTATUS);
	rtems_vector_number vector = BSP_MAX_INT;

	switch(regval){
		case (1<<BSP_INT_TIMER0_1) : 
			vector = BSP_INT_TIMER0_1; 
			break;
		case (1<<BSP_INT_TIMER2_3) : 
			vector = BSP_INT_TIMER2_3; 
			break;
		case (1<<BSP_INT_VICAP) : 
			vector = BSP_INT_VICAP; 
			break;
		case (1<<BSP_INT_GPIO_0_1_2_11):
			vector = BSP_INT_GPIO_0_1_2_11;
			break;
		case (1<<BSP_INT_GPIO_3_4_5_10):
			vector = BSP_INT_GPIO_3_4_5_10;
			break;
		case (1<<BSP_INT_GPIO_6_7_8_9):
			vector = BSP_INT_GPIO_6_7_8_9;
			break;
		case (1<<BSP_INT_IIC):
			vector = BSP_INT_IIC;
			break;
		default : 
			printk("vector=0x%x\n", regval);
			vector = BSP_MAX_INT; 
			break;		
	}

	bsp_interrupt_handler_dispatch(vector);
}

rtems_status_code bsp_interrupt_vector_enable(rtems_vector_number vector)
{
	return RTEMS_SUCCESSFUL;
}

rtems_status_code bsp_interrupt_vector_disable(rtems_vector_number vector)
{
	return RTEMS_SUCCESSFUL;
}

rtems_status_code bsp_interrupt_facility_initialize(void)
{
	INT_WR_REG(REG_INTC_INTENCLEAR, ~0);
	INT_WR_REG(REG_INTC_INTSELECT, 0);
	INT_WR_REG(REG_INTC_SOFTINTCLEAR, ~0);
	INT_WR_REG(REG_INTC_PROTECTION, 1);

	_CPU_ISR_install_vector(ARM_EXCEPTION_IRQ, _ARMV4_Exception_interrupt, NULL);

	return RTEMS_SUCCESSFUL;
}

rtems_status_code bsp_interrupt_vector_clear(rtems_vector_number vector)
{
	INT_WR_REG(REG_INTC_SOFTINTCLEAR, vector);

	return RTEMS_SUCCESSFUL;
}
