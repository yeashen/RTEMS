#
#  RTEMS_MAKEFILE_PATH is typically set in an environment variable
#
RTEMS_MAKEFILE_PATH=/home/lxm/Test/rtems/install/arm-rtems4.11/hi3518e
PGM=${ARCH}/low_ticker.exe

# optional managers required
MANAGERS=all

# C source names
CSRCS = init.c
COBJS = $(CSRCS:%.c=${ARCH}/%.o)

include $(RTEMS_MAKEFILE_PATH)/Makefile.inc
include $(RTEMS_CUSTOM)
include $(PROJECT_ROOT)/make/leaf.cfg

OBJS= $(COBJS) $(CXXOBJS) $(ASOBJS)

all:    ${ARCH} $(PGM)

$(PGM): $(OBJS)
	$(make-exe)
