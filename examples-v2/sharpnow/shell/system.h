/*  system.h
 *
 *  This include file contains information that is included in every
 *  function in the test set.
 *
 *  COPYRIGHT (c) 1989-2009.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#include <rtems.h>
#include <rtems/test.h>
/* functions */

rtems_task Init(
  rtems_task_argument argument
);

/* global variables */


/* configuration information */

#include <bsp.h> /* for device driver prototypes */

#define CONFIGURE_MAXIMUM_DRIVERS           4
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER


  #define CONFIGURE_APPLICATION_NEEDS_LIBBLOCK
  #define CONFIGURE_BDBUF_MAX_READ_AHEAD_BLOCKS  2
  #define CONFIGURE_BDBUF_MAX_WRITE_BLOCKS       8
  #define CONFIGURE_SWAPOUT_TASK_PRIORITY        15

/*
 * XXX: these values are higher than needed...
 */
#define CONFIGURE_LIBIO_MAXIMUM_FILE_DESCRIPTORS 20
#define CONFIGURE_STACK_CHECKER_ENABLED
#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_MALLOC_STATISTICS

#define CONFIGURE_UNIFIED_WORK_AREAS
#define CONFIGURE_UNLIMITED_OBJECTS

#define CONFIGURE_INITIAL_EXTENSIONS RTEMS_TEST_INITIAL_EXTENSION

#define CONFIGURE_SHELL_COMMANDS_INIT
#define CONFIGURE_SHELL_COMMANDS_ALL

#include <rtems/shellconfig.h>
#include <rtems/confdefs.h>

/* end of include file */
