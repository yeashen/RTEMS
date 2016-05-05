# RTEMS_CHECK_BSPDIR(RTEMS_BSP_FAMILY)
AC_DEFUN([RTEMS_CHECK_BSPDIR],
[
  case "$1" in
  csb337 )
    AC_CONFIG_SUBDIRS([csb337]);;
  gp32 )
    AC_CONFIG_SUBDIRS([gp32]);;
  hi3518e )
    AC_CONFIG_SUBDIRS([hi3518e]);;
  lpc32xx )
    AC_CONFIG_SUBDIRS([lpc32xx]);;
  smdk2410 )
    AC_CONFIG_SUBDIRS([smdk2410]);;
  stm32f4 )
    AC_CONFIG_SUBDIRS([stm32f4]);;
  xilinx-zynq )
    AC_CONFIG_SUBDIRS([xilinx-zynq]);;
  *)
    AC_MSG_ERROR([Invalid BSP]);;
  esac
])
