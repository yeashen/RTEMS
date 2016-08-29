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

#include "system.h"
	
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

rtems_task Task_2(rtems_task_argument task_index)
{
	char data_name[40];
	buf_info b_info;
	FILE *w_fd = NULL;
	int status = 0;
	int i = 0;
	
	sprintf(data_name, "/data/data_0");
	if((w_fd =fopen(data_name,"w")) == NULL)
	{
		printf("\nerror on open %s", data_name);
		exit(0);
	}
	//fwrite(d_data, sizeof(unsigned char), 1024, w_fd);
	fwrite(buffer_addr_y[0], sizeof(unsigned char), VI_IMG_WIDTH*VI_IMG_HEIGHT, w_fd);
	fclose(w_fd);

	status = rtems_event_send(Task_id[3], RTEMS_EVENT_15);
	CHECK_RET(status, "rtems_event_send");

	for ( ; ; ) {
		status = video_capture_ioctl(VICAP_GET_DATA_STA, &b_info);
		if(status > 0)
		{
			//printf("catch: cur=%d, valid=%d, w_pos=%d\n", b_info.cur_num, b_info.valid_num,b_info.w_pos);
		}else{
			printf("get data failed\n");
		}
		//(void) rtems_task_wake_after(task_index*rtems_clock_get_ticks_per_second());
	}
}

#define BUFFER_SIZE 1024 
rtems_task Task_3(rtems_task_argument task_index)
{
	char data_name[40];
	char buffer[BUFFER_SIZE]; 
    bzero(buffer, BUFFER_SIZE); 
	int length = 0; 
	FILE *r_fd;
	int status = 0;
	rtems_event_set   eventout;

	status = rtems_event_receive(
	    RTEMS_EVENT_15,
	    RTEMS_DEFAULT_OPTIONS,
	    RTEMS_NO_TIMEOUT,
	    &eventout
  	);
	CHECK_RET(status,"rtems_event_receive\n");
#if 0	
	sprintf(data_name, "/data/data_0");
	r_fd = fopen(data_name, "r"); 
	if(NULL == r_fd) 
	{ 
	  printf("File:%s Not Found\n", data_name); 
	} 
	else
	{
		while((length = fread(buffer, sizeof(char), BUFFER_SIZE, r_fd)) > 0) 
		{ 
			hi_uart_fns.deviceWrite(2, buffer, length);
			bzero(buffer, BUFFER_SIZE); 
		} 
	}
#endif

	for ( ; ; ) {
		//printf("Func: %s\n", __FUNCTION__);
		(void) rtems_task_wake_after(task_index*rtems_clock_get_ticks_per_second());
	}
}