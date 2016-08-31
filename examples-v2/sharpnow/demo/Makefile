#
#  RTEMS_MAKEFILE_PATH is typically set in an environment variable
#
RTEMS_MAKEFILE_PATH=$(HOME)/hi3518e/rtems/install/arm-rtems4.11/hi3518e
PGM=${ARCH}/camera.exe
RTEMS_TFTP=/home/share/tftp
MKIMG=$(HOME)/hi3518e/rtems/rtems/examples-v2/mkimage

# optional managers required
MANAGERS=all

# C source names
CSRCS = init.c main.c hireg.c
COBJS = $(CSRCS:%.c=${ARCH}/%.o)

H_FILES=system.h

include $(RTEMS_MAKEFILE_PATH)/Makefile.inc
include $(RTEMS_CUSTOM)
include $(PROJECT_ROOT)/make/leaf.cfg

OBJS= $(COBJS) $(CXXOBJS) $(ASOBJS)

all:    ${ARCH} $(PGM)

$(PGM): $(OBJS)
	$(make-exe)
	arm-rtems4.11-objcopy -R -S -O binary ${ARCH}/camera.exe ${RTEMS_TFTP}/kernel
	#arm-rtems4.11-objcopy -R -S -O binary ${ARCH}/camera.exe ${ARCH}/camera.bin
	#$(MKIMG)  -A arm -O rtems -T kernel -C none -a 0x82000000 -e 0x82000b84 -n RTEMS -d ${ARCH}/camera.bin ${RTEMS_TFTP}/kernel

