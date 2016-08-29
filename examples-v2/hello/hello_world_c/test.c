/*
 *  Classic API Hello World
 */

#include <rtems.h>
#include <stdlib.h>
#include <stdio.h>

#include <rtems/test.h>
#include <libchip/ov9712.h>
#include <libchip/vicap.h>
#include <rtems/shell.h>	

rtems_name task_name[3];
rtems_id task_id[3];

#define STA_CHECK(status, msg)	 \
do{ \
	if(status != RTEMS_SUCCESSFUL){ \
		printf("%s failed, status=%d\n", msg, status); \
	} \
}while(0)

rtems_task task_0(rtems_task_argument argument)
{
	int status;

	for(;;)
	{
		printf("%s\n", __FUNCTION__);
		(void) rtems_task_wake_after(rtems_clock_get_ticks_per_second());
	}
}

rtems_task task_1(rtems_task_argument argument)
{
	int status;

	for(;;)
	{
		printf("%s\n", __FUNCTION__);
		(void) rtems_task_wake_after(rtems_clock_get_ticks_per_second());
	}
}

rtems_task Init(
  rtems_task_argument ignored
)
{
	int status;
	sensor_data_s i2c_data;
	vicap_para_s vicap_para;

	vicap_para.stDesY.buf_len = 1280*720;
	vicap_para.stDesY.width = 1280;
	vicap_para.stDesY.height = 720;
	vicap_para.stDesY.stride = 1280;

	video_capture_open();
	video_capture_init(&vicap_para);

	sensor_ov9712_init();
#if 1
	uint32_t i, reg= 0x17;
	for(i = 0; i < 4; i++)
	{
		i2c_data.reg_addr = reg+i;
		i2c_data.data = 0x0;
		sensor_ov9712_read(&i2c_data);
		printf("reg:0x%x->0x%x\n", i2c_data.reg_addr,  i2c_data.data&0xff);
		(void) rtems_task_wake_after(rtems_clock_get_ticks_per_second()/5);
	}
#endif
	task_name[1] = rtems_build_name( 'T', 'A', '0', ' ' );
	task_name[2] = rtems_build_name( 'T', 'A', '1', ' ' );
	
	status = rtems_task_create(
					task_name[1],
					1,
					RTEMS_MINIMUM_STACK_SIZE * 2,
					RTEMS_DEFAULT_MODES,
					RTEMS_DEFAULT_ATTRIBUTES,
					&task_id[1]
	);
	STA_CHECK(status, "rtems_task_create 1");
	 
	status = rtems_task_create(
					 task_name[2],
					 1,
					 RTEMS_MINIMUM_STACK_SIZE * 2,
					 RTEMS_DEFAULT_MODES,
					 RTEMS_DEFAULT_ATTRIBUTES,
					 &task_id[2]
	);
	STA_CHECK(status, "rtems_task_create 2");

	 rtems_task_start(task_id[1], task_0, 1);
	 rtems_task_start(task_id[2], task_1, 2);
	
	(void) rtems_task_delete( RTEMS_SELF );
}

/* configuration information */

#include <bsp.h>

/* NOTICE: the clock driver is explicitly disabled */
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_USE_DEVFS_AS_BASE_FILESYSTEM

#define CONFIGURE_EXTRA_TASK_STACKS         (2 * RTEMS_MINIMUM_STACK_SIZE)
#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
#define CONFIGURE_MAXIMUM_TASKS						3
#define CONFIGURE_MAXIMUM_SEMAPHORES			2

#define CONFIGURE_INIT
#include <rtems/confdefs.h>
/* end of file */
