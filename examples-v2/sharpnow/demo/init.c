/******************************************************************************

	Copyright (C), 2015-2025, SHARPNOW Co., Ltd.

 ******************************************************************************
  File Name     : init.c
  Version       : Initial Draft
  Author        : Juven
  Created       : 2016/05/13
  Last Modified : 
  Description   : 
  History       : 
  1.Date        : 2016/05/13
   Author       : Juven
   Modification : Created file

******************************************************************************/
#define CONFIGURE_INIT

#include "system.h"
#include <inttypes.h>
#include <stdio.h>

#ifdef SNS_POA030R
#include <libchip/poa030r.h>
#else
#include <libchip/ov9712.h>
#endif
#include <libchip/vicap.h>
#include <libchip/gpio.h>
#include <libchip/virreg_hal.h>

rtems_id   Task_id[4];         /* array of task ids */
rtems_name Task_name[4];       /* array of task names */

rtems_id   Partition_id[4];   /* array of partition ids */
rtems_name Partition_name[4]; /* array of partition names */
uint8_t   AreaY[3][IMG_SIZE] CPU_STRUCTURE_ALIGNMENT;
uint8_t   AreaC[IMG_SIZE] CPU_STRUCTURE_ALIGNMENT;
void              *buffer_addr_y[3];
void              *buffer_addr_c;

static void GpioInit()
{
	gpio_set_derection(GPIO3, GPIO_PIN5, GPIO_OUTPUT);//sensor standby
	gpio_set_derection(GPIO0, GPIO_PIN5, GPIO_OUTPUT);//sensor reset
	gpio_set_derection(GPIO1, GPIO_PIN2, GPIO_OUTPUT);//irled
	gpio_set_derection(GPIO4, GPIO_PIN3, GPIO_OUTPUT);//ready
	gpio_set_derection(GPIO4, GPIO_PIN4, GPIO_OUTPUT);//irq
	gpio_set(GPIO4, GPIO_PIN3, 1);	
	gpio_set(GPIO4, GPIO_PIN4, 1);	
	gpio_set(GPIO1, GPIO_PIN2, 0);	
	gpio_set(GPIO0, GPIO_PIN5, 1);	
	gpio_set(GPIO3, GPIO_PIN5, 0);   
	gpio_set(GPIO5, GPIO_PIN0, 0);	//led0
	gpio_set(GPIO0, GPIO_PIN5, 0);	//reset
	usleep(10);
	gpio_set(GPIO0, GPIO_PIN5, 1);	
}

rtems_task Init( rtems_task_argument argument)
{
	int status;
	int i = 0;
	sensor_type_e sensor_type;

	/* create task */
	Task_name[ 1 ] = rtems_build_name( 'T', 'A', '1', ' ' );
	Task_name[ 2 ] = rtems_build_name( 'T', 'A', '2', ' ' );
	Task_name[ 3 ] = rtems_build_name( 'T', 'A', '3', ' ' );

	(void) rtems_task_create(
		Task_name[ 1 ], 
		1, 
		RTEMS_MINIMUM_STACK_SIZE * 2, 
		RTEMS_DEFAULT_MODES,
		RTEMS_DEFAULT_ATTRIBUTES, 
		&Task_id[ 1 ]
	);
	(void) rtems_task_create(
		Task_name[ 2 ], 
		2, 
		RTEMS_MINIMUM_STACK_SIZE * 2,
		RTEMS_DEFAULT_MODES,
		RTEMS_DEFAULT_ATTRIBUTES, 
		&Task_id[ 2 ]
	);
	(void) rtems_task_create(
		Task_name[ 3 ], 
		3, 
		RTEMS_MINIMUM_STACK_SIZE * 2, 
		RTEMS_DEFAULT_MODES,
		RTEMS_DEFAULT_ATTRIBUTES, 
		&Task_id[ 3 ]
	);

	/* init virtual reg */
	virtualreg_init_reg();
	
	/* create pation */
	for(i = 0; i < 3; i++)
	{
		Partition_name[i] =  rtems_build_name( 'P', 'T', i, ' ' );
		(void)rtems_partition_create(
		    Partition_name[i],
		    AreaY[i],
		    IMG_SIZE,
		    IMG_SIZE,
		    RTEMS_DEFAULT_ATTRIBUTES,
		    &Partition_id[i]
		);

		status = rtems_partition_get_buffer( Partition_id[i], &buffer_addr_y[i]);
		CHECK_RET(status, "rtems_partition_get_buffer Y");
		//printf("y_addr: %p\n", buffer_addr_y[i]);
	}
	
	Partition_name[i] =  rtems_build_name( 'P', 'T', i, ' ' );
	(void)rtems_partition_create(
	    Partition_name[i],
	    AreaC,
	    IMG_SIZE,
	    IMG_SIZE,
	    RTEMS_DEFAULT_ATTRIBUTES,
	    &Partition_id[i]
	);

	status = rtems_partition_get_buffer( Partition_id[i], &buffer_addr_c);
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

	GpioInit();

#ifdef SNS_POA030R
	sensor_type = SENSOR_POA030R;
	sensor_poa030r_init(MODE_320x240_30FPS);
	//sensor_poa030r_init(MODE_640x480_30FPS);
#endif
	video_capture_open(Task_id[2]);
	video_capture_init(sensor_type, &vicap_para);
	video_capture_ioctl(VICAP_START_CAPTURE, 0);

	(void) rtems_task_start( Task_id[ 1 ], Task_1, 1 );
	(void) rtems_task_start( Task_id[ 2 ], Task_2, 2 );
	(void) rtems_task_start( Task_id[ 3 ], Task_3, 3 );

	gpio_set(GPIO4, GPIO_PIN3, 0);
	usleep(10);
	gpio_set(GPIO4, GPIO_PIN3, 1);
	
	(void) rtems_task_delete( RTEMS_SELF );
}

