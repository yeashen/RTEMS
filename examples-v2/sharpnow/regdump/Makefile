#
#  RTEMS_MAKEFILE_PATH is typically set in an environment variable
#
RTEMS_MAKEFILE_PATH=/home/lxm/Test/rtems/install/arm-rtems4.11/hi3518e
PGM=${ARCH}/regdump.exe
RTEMS_TFTP=/home/lxm/Test/hi3518e

# optional managers required
MANAGERS=all

# C source names
CSRCS = regdump.c
COBJS = $(CSRCS:%.c=${ARCH}/%.o)

include $(RTEMS_MAKEFILE_PATH)/Makefile.inc
include $(RTEMS_CUSTOM)
include $(PROJECT_ROOT)/make/leaf.cfg

OBJS= $(COBJS) $(CXXOBJS) $(ASOBJS)

all:    ${ARCH} $(PGM)

$(PGM): $(OBJS)
	$(make-exe)
	arm-rtems4.11-objcopy -O binary -S ${ARCH}/regdump.exe ${RTEMS_TFTP}/kernel
