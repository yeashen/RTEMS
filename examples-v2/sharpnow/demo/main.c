/*
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 */
#define CONFIGURE_SHELL_COMMANDS_INIT

#include <stdio.h>
#include <stdlib.h>
#include <rtems/libcsupport.h>
#include <rtems/shell.h>

#include <uart.h>
#include <libchip/gpio.h>
#include <libchip/vicap.h>
#include <libchip/pkthandle.h>

#include "system.h"
#if 0
rtems_task Task_1(rtems_task_argument task_index)
{
	int sc;

	sc = mkdir("/data", 0777);
	if ( sc ) {
		printf( "mkdir /data: %s:\n", strerror(errno) );
	}

	/* debug hireg */
	rtems_shell_add_cmd ("himd", "misc", "display physic reg value", himd_reg);
	rtems_shell_add_cmd ("himm", "misc", "modify physic reg value", himm_reg);

	/* debug i2c */
	rtems_shell_add_cmd ("i2crd", "misc", "read i2c slave reg value", i2creg_read);
	rtems_shell_add_cmd ("i2cwr", "misc", "write i2c slave reg value", i2creg_write);

	/* debug reboot */
	rtems_shell_add_cmd ("reboot", "misc", "reboot system", system_reboot);

	/* debug vicap */
	rtems_shell_add_cmd ("catvi", "misc", "show vicap capture frame num", show_vicap_int);
	
	rtems_shell_init(
		"SHLL",                          /* task_name */
		RTEMS_MINIMUM_STACK_SIZE * 4,    /* task_stacksize */
		100,                             /* task_priority */
		"/dev/hi3518e",                   /* devname */
		/* device is currently ignored by the shell if it is not a pty */
		false,                           /* forever */
		true,                            /* wait */
		NULL          /* login */
	);
}
#else
rtems_task Task_1(rtems_task_argument task_index)
{
	unsigned char c;

	for(;;){
		c = hi_uart0_fns.deviceRead(0);
		printf("uart0 - %d\n", c);
		(void) rtems_task_wake_after(task_index*rtems_clock_get_ticks_per_second());
	}
}
#endif

rtems_task Task_2(rtems_task_argument task_index)
{
	for(;;){
		(void) rtems_task_wake_after(task_index*rtems_clock_get_ticks_per_second());
	}
}

rtems_task Task_3(rtems_task_argument task_index)
{
	unsigned char c;

	for(;;){
		c = hi_uart2_fns.deviceRead(2);
		printf("uart2 - %d\n", c);
		(void) rtems_task_wake_after(task_index*rtems_clock_get_ticks_per_second());
	}
}
