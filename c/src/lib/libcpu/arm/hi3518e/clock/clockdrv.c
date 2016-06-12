/******************************************************************************

	Copyright (C), 2015-2025, SHARPNOW Co., Ltd.

 ******************************************************************************
  File Name     : clock.c
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
#include <rtems.h>
#include <bsp/irq.h>
#include <bsp.h>
#include <hi3518e.h>

#define CFG_TIMER_ENABLE			(1 << 7)
#define CFG_TIMER_PERIODIC		(1 << 6)
#define CFG_TIMER_INTMASK		(1 << 5)
#define CFG_TIMER_32BIT			(1 << 1)

#define CFG_TIMER_IRQEN			(1<<3)

#define CFG_TIMER_CONTROL		(CFG_TIMER_ENABLE | CFG_TIMER_PERIODIC\
					| CFG_TIMER_INTMASK | CFG_TIMER_32BIT)

#define CFG_TIMER_PRESCALE		2 /* AXI:APB = 2:1 */
#define HZ									100 /* 10ms */
#define BUSCLK_TO_TIMER_RELOAD(busclk)	 (((busclk)/CFG_TIMER_PRESCALE)/HZ)

void Clock_isr(rtems_irq_hdl_param arg);
static void clock_isr_on(const rtems_irq_connect_data *unused);
static void clock_isr_off(const rtems_irq_connect_data *unused);
static int clock_isr_is_on(const rtems_irq_connect_data *irq);

static hi_timer_regs_s *timer0_reg = (hi_timer_regs_s *)TIMER0_REG_BASE;
static hi_sysctrl_regs_s *sysctrl_reg = (hi_sysctrl_regs_s *)SYSCTRL_REG_BASE;

rtems_irq_connect_data clock_isr_data = {
	.name 	= BSP_INT_TIMER0_1,
	.hdl		= Clock_isr,
	.handle = NULL,
	.on 		= clock_isr_on,
	.off		= clock_isr_off,
	.isOn 	= clock_isr_is_on,
};

/**
 * When we get the clock interrupt
 *		- clear the interrupt bit?
 *		- restart the timer?
 */
#define Clock_driver_support_at_tick()                \
	do {																							\
		timer0_reg->icr = 0x0; \
	} while(0)


/**
 * Installs the clock ISR. You shouldn't need to change this.
 */
#define Clock_driver_support_install_isr( _new, _old ) \
	do {																								 \
		_old = NULL;																			 \
		BSP_install_rtems_irq_handler(&clock_isr_data); 	 \
	} while(0)


/**
 * Initialize the hardware for the clock
 *	 - Set the frequency
 *	 - enable it
 *	 - clear any pending interrupts
 *
 * Since you may want the clock always running, you can
 * enable interrupts here. If you do so, the clock_isr_on(),
 * clock_isr_off(), and clock_isr_is_on() functions can be
 * NOPs.
 */

#define Clock_driver_support_initialize_hardware() \
	do { \
		/* set clock source busclk */ \
		sysctrl_reg->sc_lock = SCPER_LOCK; \
		sysctrl_reg->sc_ctrl = (sysctrl_reg->sc_ctrl |  (1<<16) | (1<<18) | (1<<20)); \
		sysctrl_reg->sc_lock = 0x0; \
		timer0_reg->ctrl = 0x0; \
		timer0_reg->load = BUSCLK_TO_TIMER_RELOAD(CFG_CLK_BUS); \
		timer0_reg->ctrl = CFG_TIMER_CONTROL;\
		} while (0)

/**
 * Do whatever you need to shut the clock down and remove the
 * interrupt handler. Since this normally only gets called on
 * RTEMS shutdown, you may not need to do anything other than
 * remove the ISR.
 */
#define Clock_driver_support_shutdown_hardware()                        \
	do {																																	\
				/* Disable timer */ \
				BSP_remove_rtems_irq_handler(&clock_isr_data);									\
		 } while (0)

/**
 * Enables clock interrupt.
 *
 * If the interrupt is always on, this can be a NOP.
 */
static void clock_isr_on(const rtems_irq_connect_data *unused)
{
}

/**
 * Disables clock interrupts
 *
 * If the interrupt is always on, this can be a NOP.
 */
static void clock_isr_off(const rtems_irq_connect_data *unused)
{
}

/**
 * Tests to see if clock interrupt is enabled, and returns 1 if so.
 * If interrupt is not enabled, returns 0.
 *
 * If the interrupt is always on, this always returns 1.
 */
static int clock_isr_is_on(const rtems_irq_connect_data *irq)
{
	return 1;
}

#define CLOCK_DRIVER_USE_DUMMY_TIMECOUNTER

/* Make sure to include this, and only at the end of the file */
#include "../../../../libbsp/shared/clockdrv_shell.h"

