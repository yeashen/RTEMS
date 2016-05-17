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

#include <hi3518e.h>

static hi_irq_regs_s *irq_reg = NULL;

void bsp_interrupt_dispatch(void)
{
	volatile uint32_t regval = irq_reg->irq_status;
	rtems_vector_number vector = BSP_MAX_INT;

	switch(regval){
		case (1<<BSP_INT_TIMER0_1) : 
			vector = BSP_INT_TIMER0_1; 
			break;
		case (1<<BSP_INT_VICAP) : 
			vector = BSP_INT_VICAP; 
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
	irq_reg->enable = 1<<vector;
		
	return RTEMS_SUCCESSFUL;
}


rtems_status_code bsp_interrupt_vector_disable(rtems_vector_number vector)
{

	irq_reg->en_clr = 1<<vector;
		
	return RTEMS_SUCCESSFUL;
}

rtems_status_code bsp_interrupt_facility_initialize(void)
{
	irq_reg = (hi_irq_regs_s *)IRQ_REG_BASE;

	irq_reg->en_clr = 0xFFFFFFFF;
	irq_reg->src = 0x0;
	irq_reg->softint = 0x0;
	irq_reg->protect = 0x0;

	_CPU_ISR_install_vector(ARM_EXCEPTION_IRQ, _ARMV4_Exception_interrupt, NULL);

	return RTEMS_SUCCESSFUL;
}
