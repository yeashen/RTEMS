/*
 *  COPYRIGHT (c) 1989-2011.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */


#ifndef _SYSTEM_H
#define _SYSTEM_H

rtems_task Test_task(
  rtems_task_argument task_index
)£»

rtems_task Init(
  rtems_task_argument argument
)£»


/**************** START OF CONFIGURATION INFORMATION ****************/

#define CONFIGURE_INIT

#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_DISABLE_FILESYSTEM
#define CONFIGURE_DISABLE_NEWLIB_REENTRANCY
#define CONFIGURE_TERMIOS_DISABLED
#define CONFIGURE_LIBIO_MAXIMUM_FILE_DESCRIPTORS 0
#define CONFIGURE_MINIMUM_TASK_STACK_SIZE 512
#define CONFIGURE_MAXIMUM_PRIORITY 15
#define CONFIGURE_DISABLE_CLASSIC_API_NOTEPADS
#define CONFIGURE_IDLE_TASK_BODY Init
#define CONFIGURE_IDLE_TASK_INITIALIZES_APPLICATION

#define CONFIGURE_MAXIMUM_TASKS             3

#include <rtems/confdefs.h>

/****************  END OF CONFIGURATION INFORMATION  ****************/



#endif /* _SYSTEM_H */
