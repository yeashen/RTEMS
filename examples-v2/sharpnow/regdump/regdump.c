/*
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <bsp.h>
#include <libchip/vicap.h>
#include <libchip/gpio.h>

#include <debug.h>
#include "system.h"

#ifdef SNS_POA030R
#include <libchip/poa030r.h>
#else
#include <libchip/ov9712.h>
#endif

rtems_id   Task_id[2];         /* array of task ids */
rtems_name Task_name[2];       /* array of task names */

rtems_id   Partition_id[4];   /* array of partition ids */
rtems_name Partition_name[4]; /* array of partition names */
uint8_t   AreaY[3][IMG_SIZE] CPU_STRUCTURE_ALIGNMENT;
uint8_t   AreaC[IMG_SIZE] CPU_STRUCTURE_ALIGNMENT;
void              *buffer_addr_y[3];
void              *buffer_addr_c;

#define CHECK_RET(status, msg)	 \
do{ \
	if(status != RTEMS_SUCCESSFUL){\
		printf("%s failed, status=%d", msg, status); \
		exit(0); \
	} \
}while(0)


int hex_atoi(char * pstr)
{
	int res = 0;
	int sign = 1;

	if(pstr == NULL)
	{
		printf("Point is NULL");
		return -1;
	}

	while(*pstr == ' ')
		pstr++;
	if(*pstr == '-')
		sign = -1;
	if(*pstr == '-' || *pstr == '+')
		pstr++;

	while((*pstr >= '0' && *pstr <= '9') || (*pstr >= 'a' && *pstr <= 'f')
				|| (*pstr >= 'A' && *pstr <= 'F'))
	{
		if(*pstr >= '0' && *pstr <= '9')
			res = res * 16 + *pstr - '0';
		else if(*pstr >= 'a' && *pstr <= 'f')
			res = res * 16 + *pstr - '0' - 39;
		else
			res = res * 16 + *pstr - '0' - 7;
		pstr++;
	}
	res *= sign;

	return res;
}

rtems_task Task_1(
  rtems_task_argument task_index
)
{
	int status, i = 0;
	char reg_str[12];
	unsigned int reg_base = 0x20580000, reg;
	unsigned int val = 0;

#if 1
	for(i = 0; i < 0x1100; i += 4)
	{
		reg = reg_base + i;
		val = phyreg_read(reg);
		printf("reg: 0x%8X --> 0x%8X\n", reg, val);
	}
#endif

	for ( ; ; ) {
		printf("addr:");
		fflush(stdout);
		gets(reg_str);
	    reg_base = hex_atoi(reg_str);
		val = phyreg_read(reg_base);
		printf("reg: 0x%8X --> 0x%8X\n", reg_base, val);
		//(void) rtems_task_wake_after(task_index*rtems_clock_get_ticks_per_second());
	}
}

rtems_task Init(
  rtems_task_argument argument
)
{
	int status, i;
	sensor_type_e sensor_type;

	rtems_time_of_day time;

	time.year   = 2016;
	time.month  = 4;
	time.day    = 6;
	time.hour   = 14;
	time.minute = 0;
	time.second = 0;
	time.ticks  = 0;

	(void) rtems_clock_set( &time );

	/* create pation */
	Partition_name[ 1 ] =  rtems_build_name( 'P', 'T', '1', ' ' );
	(void)rtems_partition_create(
	    Partition_name[1],
	    AreaY,
	    IMG_SIZE,
	    IMG_SIZE,
	    RTEMS_DEFAULT_ATTRIBUTES,
	    &Partition_id[1]
	);

	Partition_name[ 2 ] =  rtems_build_name( 'P', 'T', '2', ' ' );
	(void)rtems_partition_create(
	    Partition_name[2],
	    AreaC,
	    IMG_SIZE,
	    IMG_SIZE,
	    RTEMS_DEFAULT_ATTRIBUTES,
	    &Partition_id[2]
	);

	status = rtems_partition_get_buffer( Partition_id[1], &buffer_addr_y);
	CHECK_RET(status, "rtems_partition_get_buffer Y");

	status = rtems_partition_get_buffer( Partition_id[2], &buffer_addr_c);
	CHECK_RET(status, "rtems_partition_get_buffer C");

	
	vicap_para_s vicap_para;
	
	vicap_para.width = VI_IMG_WIDTH;
	vicap_para.height = VI_IMG_HEIGHT;
	vicap_para.caddr = (unsigned int)buffer_addr_c;
	for(i = 0; i < 3; i++)
	{
		vicap_para.yaddr[i] = (unsigned int)buffer_addr_y[i];
	}

#ifdef SNS_OV9712
	sensor_type = SENSOR_OV9712;
	sensor_ov9712_init(MODE_640x400);
#endif

#ifdef SNS_POA030R
	sensor_type = SENSOR_POA030R;
	gpio_set_derection(GPIO3, GPIO_PIN5, GPIO_OUTPUT);//sensor standby
	gpio_set_derection(GPIO0, GPIO_PIN5, GPIO_OUTPUT);//sensor reset
	gpio_set(GPIO0, GPIO_PIN5, 1);	
	gpio_set(GPIO3, GPIO_PIN5, 0);	 
	gpio_set(GPIO5, GPIO_PIN0, 0);	//led0
	gpio_set(GPIO0, GPIO_PIN5, 0);	//reset
	usleep(10);
	gpio_set(GPIO0, GPIO_PIN5, 1);	
	//sensor_poa030r_init(MODE_320x240_30FPS);
	sensor_poa030r_init(MODE_640x480_30FPS);
#endif
	video_capture_open(Task_id[2]);
	video_capture_init(sensor_type, &vicap_para);
	video_capture_ioctl(VICAP_START_CAPTURE, 0);
	

	Task_name[ 1 ] = rtems_build_name( 'T', 'A', '1', ' ' );

	(void) rtems_task_create(
		Task_name[ 1 ], 
		1, 
		RTEMS_MINIMUM_STACK_SIZE * 2, 
		RTEMS_DEFAULT_MODES,
		RTEMS_DEFAULT_ATTRIBUTES, 
		&Task_id[ 1 ]
	);

	(void) rtems_task_start( Task_id[ 1 ], Task_1, 1 );

	(void) rtems_task_delete( RTEMS_SELF );
}

