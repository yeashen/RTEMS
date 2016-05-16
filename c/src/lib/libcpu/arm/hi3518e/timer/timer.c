/******************************************************************************

	Copyright (C), 2015-2025, SHARPNOW Co., Ltd.

 ******************************************************************************
  File Name     : timer.c
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
#include <rtems.h>
#include <rtems/btimer.h>
#include <hi3518e.h>

bool benchmark_timer_find_average_overhead;

#define TIMER_LOAD_VAL 0xffffffff

static unsigned long timestamp; /* how long since last timer reset */
static unsigned long lastdec;

void reset_timer_masked(void)
{
	/* reset time */
	lastdec = TIMER1_RD_REG(REG_TIMER_VALUE);  /* capure current decrementer value time */
	timestamp = 0;	       /* start "advancing" time stamp from 0 */
}

/*
 * Set up Timer 1
 */
void benchmark_timer_initialize( void )
{
	/* enable the reference clk. but we did not find the reset switch */
	HI_REG_WR(REG_BASE_SCTL+REG_SC_CTRL, HI_REG_RD(REG_BASE_SCTL+REG_SC_CTRL) \
					| (1<<16) | (1<<18) | (1<<20));
	
	/*
	 * Under uboot, 0xffffffff is set to load register,
	 * timer_clk = BUSCLK/2/256.
	 * e.g. BUSCLK = 50M, it will roll back after 0xffffffff/timer_clk
	 * = 43980s = 12hours
	 */
	TIMER1_WR_REG(REG_TIMER_CONTROL, 0);
	TIMER1_WR_REG(REG_TIMER_RELOAD, ~0);

	/* 32 bit, periodic,	256 divider. */
	TIMER1_WR_REG(REG_TIMER_CONTROL, 0xCA);

	/* init the timestamp and lastdec value */
	reset_timer_masked();
}

/*
 *	The following controls the behavior of benchmark_timer_read().
 *
 *	AVG_OVEREHAD is the overhead for starting and stopping the timer.  It
 *	is usually deducted from the number returned.
 *
 *	LEAST_VALID is the lowest number this routine should trust.  Numbers
 *	below this are "noise" and zero is returned.
 */

benchmark_timer_t benchmark_timer_read( void )
{
	/*
	 *	Total is calculated by taking into account the number of timer overflow
	 *	interrupts since the timer was initialized and clicks since the last
	 *	interrupts.
	 */
	unsigned long now = TIMER1_RD_REG(REG_TIMER_VALUE); 	/* current tick value */
	
		if (lastdec >= now) {
			/* normal mode, not roll back */
			timestamp += lastdec - now;
		} else {
			/* rollback */
			timestamp += lastdec + TIMER_LOAD_VAL - now;
		}
		lastdec = now;
	
		return timestamp;

}

void benchmark_timer_disable_subtracting_average_overhead(bool find_flag)
{
	benchmark_timer_find_average_overhead = find_flag;
}

