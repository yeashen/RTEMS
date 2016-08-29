/*
 *  COPYRIGHT (c) 1989-2012.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define CONFIGURE_INIT
#include "system.h"
#include <sys/param.h>
#include <crypt.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <rtems.h>
#include <fcntl.h>
#include <inttypes.h>
#include <rtems/error.h>
#include <rtems/dosfs.h>
#include <ctype.h>
#include <rtems/libcsupport.h>
#include <rtems/shell.h>

static void fileio_start_shell(void)
{
  printf("\n =========================\n");
  printf(" starting shell\n");
  printf(" =========================\n");
  rtems_shell_init(
    "SHLL",                          /* task_name */
    RTEMS_MINIMUM_STACK_SIZE * 4,    /* task_stacksize */
    100,                             /* task_priority */
    "/dev/foobar",                   /* devname */
    /* device is currently ignored by the shell if it is not a pty */
    false,                           /* forever */
    true,                            /* wait */
    NULL          /* login */
  );
}

static void
notification (int fd, int seconds_remaining, void *arg)
{
  printf(
    "Press any key to start file I/O sample (%is remaining)\n",
    seconds_remaining
  );
}

/*
 * RTEMS Startup Task
 */
rtems_task
Init (rtems_task_argument ignored)
{
  rtems_name Task_name;
  rtems_id   Task_id;
  rtems_status_code status;

  status = rtems_shell_wait_for_input(
    STDIN_FILENO,
    20,
    notification,
    NULL
  );
  if (status == RTEMS_SUCCESSFUL) {
    Task_name = rtems_build_name('F','M','N','U');

    status = rtems_task_create(
      Task_name, 1, RTEMS_MINIMUM_STACK_SIZE * 2,
      RTEMS_DEFAULT_MODES ,
      RTEMS_FLOATING_POINT | RTEMS_DEFAULT_ATTRIBUTES, &Task_id
    );

    status = rtems_task_start( Task_id, fileio_start_shell, 1 );

    status = rtems_task_delete( RTEMS_SELF );
  } 
}
