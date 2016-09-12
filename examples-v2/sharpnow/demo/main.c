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

	video_capture_ioctl(VICAP_START_CAPTURE, 0);
	
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
char data_name[40] = "/data/data_0";
static unsigned char irled = 1;

rtems_task Task_1(rtems_task_argument task_index)
{
	int status = 0;
	buf_info b_info;
	unsigned char irled = 1;
	unsigned char start_flag = 0;
	unsigned int start = 0;
	int sc;
	FILE *w_fd = NULL;

	sc = mkdir("/data", 0777);
	if ( sc ) {
		printf( "mkdir /data: %s:\n", strerror(errno) );
	}

	for(;;){
		virreg_get_sys_start(&start);
		if(SYS_START == start)
		{
			if(!start_flag)
			{
				video_capture_ioctl(VICAP_START_CAPTURE, 0);
				start_flag = 1;
			}
			status = video_capture_ioctl(VICAP_GET_DATA_STA, &b_info);
			if(status > 0)
			{
				gpio_set(GPIO1, GPIO_PIN2, irled);
				irled = !irled;
				#if 0
				if((w_fd =fopen(data_name,"w")) == NULL)
				{
					printf("\nerror on open %s", data_name);
					exit(0);
				}
				fwrite(buffer_addr_y[0], sizeof(unsigned char), VI_IMG_WIDTH*VI_IMG_HEIGHT, w_fd);
				fclose(w_fd);

				status = rtems_event_send(Task_id[3], RTEMS_EVENT_15);
				CHECK_RET(status, "rtems_event_send");
				#endif
				//printf("catch: cur=%d, valid=%d, w_pos=%d\n", b_info.cur_num, b_info.valid_num,b_info.w_pos);
			}else{
				//printf("get data failed\n");
			}
		}
		else
		{
			video_capture_ioctl(VICAP_STOP_CAPTURE, 0);
			start_flag = 0;
		}
		(void) rtems_task_wake_after(rtems_clock_get_ticks_per_second()/100);
	}
}

#endif

/*
  * This task is for system status control, Ex. reset/start/stop, and so on
  */
rtems_task Task_2(rtems_task_argument task_index)
{
	unsigned int reset = 0;

	for(;;){
		virreg_get_sys_reset(&reset);
		if(reset)
		{
			system_reboot(0, NULL);
		}
		(void) rtems_task_wake_after(task_index*rtems_clock_get_ticks_per_second());
	}
}

#if 0
#define BUFFER_SIZE 1024 
rtems_task Task_3(rtems_task_argument task_index)
{
	char buffer[BUFFER_SIZE]; 
    bzero(buffer, BUFFER_SIZE); 
	int length = 0; 
	FILE *r_fd;
	int status = 0;
	rtems_event_set   eventout;
	char start[] = {0x18};

	status = rtems_event_receive(
	    RTEMS_EVENT_15,
	    RTEMS_DEFAULT_OPTIONS,
	    RTEMS_NO_TIMEOUT,
	    &eventout
  	);
	CHECK_RET(status,"rtems_event_receive\n");

	r_fd = fopen(data_name, "r"); 
	if(NULL == r_fd) 
	{ 
	  printf("File:%s Not Found\n", data_name); 
	} 
	else
	{
		hi_uart_poll_fns.deviceWrite(2, start, 1);
	
		while((length = fread(buffer, sizeof(char), BUFFER_SIZE, r_fd)) > 0) 
		{ 
			hi_uart_fns.deviceWrite(2, buffer, length);
			bzero(buffer, BUFFER_SIZE); 
		} 
	}
	gpio_set(GPIO1, GPIO_PIN2, irled);
	
	for(;;){
		//printf("%s\n", __FUNCTION__);
		(void) rtems_task_wake_after(task_index*rtems_clock_get_ticks_per_second());
	}
}
#else
rtems_task Task_3(rtems_task_argument task_index)
{
	for(;;){
		//printf("%s\n", __FUNCTION__);
		(void) rtems_task_wake_after(task_index*rtems_clock_get_ticks_per_second());
	}
}
#endif
