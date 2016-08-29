#ifndef _SYSTEM_H
#define _SYSTEM_H

#include <inttypes.h>
#include <rtems.h>

#include <bsp.h> /* for device driver prototypes */

/* functions */

void uart_senddata(void);
int himd_reg(int argc, char *argv[]);
int himm_reg(int argc, char *argv[]);
int i2creg_read(int argc, char *argv[]);
int i2creg_write(int argc, char *argv[]);
int show_vicap_int(int argc, char *argv[]);
int system_reboot(int argc, char *argv[]);
rtems_task Init( rtems_task_argument argument);
rtems_task Task_1(rtems_task_argument task_index);
rtems_task Task_2(rtems_task_argument task_index);
rtems_task Task_3(rtems_task_argument task_index);

/* user macro */

#define CHECK_RET(status, msg)	 \
do{ \
	if(status != RTEMS_SUCCESSFUL){\
		printf("%s failed, status=%d\n", msg, status); \
		exit(0); \
	} \
}while(0)


/* global variables */

/*
 *  Keep the names and IDs in global variables so another task can use them.
 */ 
extern rtems_id   Task_id[ 4 ];         /* array of task ids */
extern rtems_name Task_name[ 4 ];       /* array of task names */

extern void              *buffer_addr_y[3];

/****************************** VIDEO CONFIGURE**************************/
//#define SNS_OV9712
#define SNS_POA030R

#ifdef SNS_OV9712
#define VI_IMG_WIDTH			640
#define VI_IMG_HEIGHT		400
#endif

#ifdef SNS_POA030R
#define VI_IMG_WIDTH			320
#define VI_IMG_HEIGHT		240
#endif

#define IMG_SIZE					(VI_IMG_WIDTH*VI_IMG_WIDTH*2)


/**************** START OF CONFIGURATION INFORMATION ****************/

#define CONFIGURE_BDBUF_MAX_READ_AHEAD_BLOCKS  2
#define CONFIGURE_BDBUF_MAX_WRITE_BLOCKS			 8
#define CONFIGURE_SWAPOUT_TASK_PRIORITY 			 15

#define CONFIGURE_SHELL_COMMANDS_ALL
#define CONFIGURE_LIBIO_MAXIMUM_FILE_DESCRIPTORS 20
#define CONFIGURE_STACK_CHECKER_ENABLED
#define CONFIGURE_MALLOC_STATISTICS
#define CONFIGURE_UNIFIED_WORK_AREAS
#define CONFIGURE_UNLIMITED_OBJECTS

#define CONFIGURE_APPLICATION_NEEDS_LIBBLOCK
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_MAXIMUM_TASKS             4

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_INIT_TASK_STACK_SIZE    (2 * RTEMS_MINIMUM_STACK_SIZE)

#define CONFIGURE_EXTRA_TASK_STACKS       (4 * RTEMS_MINIMUM_STACK_SIZE)

#define CONFIGURE_MAXIMUM_PARTITIONS          4
#include <rtems/confdefs.h>
#include <rtems/shellconfig.h>

/****************  END OF CONFIGURATION INFORMATION  ****************/


#endif /* _SYSTEM_H */
