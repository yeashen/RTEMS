#
#  Makefile
#

#
#  RTEMS_MAKEFILE_PATH is typically set in an environment variable
#
RTEMS_MAKEFILE_PATH=/home/lxm/Test/rtems/install/arm-rtems4.11/hi3518e
RTEMS_TFTP=/home/lxm/Test/hi3518e
PGM=${ARCH}/psx_example_2.exe

# optional managers required
MANAGERS=all

# C source names
CSRCS = test2.c
COBJS_ = $(CSRCS:.c=.o)
COBJS = $(COBJS_:%=${ARCH}/%)

include $(RTEMS_MAKEFILE_PATH)/Makefile.inc
include $(RTEMS_CUSTOM)
include $(PROJECT_ROOT)/make/leaf.cfg

OBJS= $(COBJS) $(CXXOBJS) $(ASOBJS)

all:    ${ARCH} $(PGM)

$(PGM): $(OBJS)
	$(make-exe)
	arm-rtems4.11-objcopy -O binary -S ${ARCH}/psx_example_2.exe ${RTEMS_TFTP}/psx_example_2.bin
