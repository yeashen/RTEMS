#
#  Makefile
#

#
#  RTEMS_MAKEFILE_PATH is typically set in an environment variable
#
RTEMS_MAKEFILE_PATH=/home/lxm/Test/rtems/install/arm-rtems4.11/hi3518e
RTEMS_TFTP=/home/lxm/Test/hi3518e
EXEC=cxx_throw.exe
PGM=${ARCH}/$(EXEC)

# optional managers required
MANAGERS=all

# C source names
CSRCS = 
COBJS_ = $(CSRCS:.c=.o)
COBJS = $(COBJS_:%=${ARCH}/%)

# C++ source names
CXXSRCS = init.cc
CXXOBJS_ = $(CXXSRCS:.cc=.o)
CXXOBJS = $(CXXOBJS_:%=${ARCH}/%)

# AS source names
ASSRCS =
ASOBJS_ = $(ASSRCS:.s=.o)
ASOBJS = $(ASOBJS_:%=${ARCH}/%)

# Libraries
LIBS = 

include $(RTEMS_MAKEFILE_PATH)/Makefile.inc

include $(RTEMS_CUSTOM)
include $(PROJECT_ROOT)/make/leaf.cfg

OBJS= $(COBJS) $(CXXOBJS) $(ASOBJS)

all:    ${ARCH} $(PGM)

$(PGM): $(OBJS)
	$(make-cxx-exe)
	arm-rtems4.11-objcopy -O binary -S ${ARCH}/cxx_throw.exe ${RTEMS_TFTP}/cxx_throw.bin
