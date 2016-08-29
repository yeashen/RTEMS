#
#  RTEMS_MAKEFILE_PATH is typically set in an environment variable
#
RTEMS_MAKEFILE_PATH=/home/lxm/Test/rtems/install/arm-rtems4.11/hi3518e
PGM=${ARCH}/fat_ramdisk.exe
RTEMS_TFTP=/home/lxm/Test/hi3518e

# optional managers required
MANAGERS=all

# C source names
CSRCS = init.c FilesystemImage.c
COBJS = $(CSRCS:%.c=${ARCH}/%.o)

CLEAN_ADDITIONS += stamp-fs-image
CLEAN_ADDITIONS += FilesystemImage FilesystemImage.c FilesystemImage.h

include $(RTEMS_MAKEFILE_PATH)/Makefile.inc
include $(RTEMS_CUSTOM)
include $(PROJECT_ROOT)/make/leaf.cfg

OBJS= $(COBJS) $(CXXOBJS) $(ASOBJS)

all:    ${ARCH} $(OBJS) $(PGM)

$(ARCH)/init.o: init.c stamp-fs-image

$(ARCH)/FilesystemImage.o: stamp-fs-image FilesystemImage.c FilesystemImage.h

FilesystemImage: $(ARCH) rootfs/shell-init
	cd rootfs ; \
	    tar cf ../FilesystemImage --exclude CVS --exclude .cvsignore .

stamp-fs-image: $(ARCH) FilesystemImage
	$(PROJECT_ROOT)/bin/rtems-bin2c FilesystemImage FilesystemImage
	ls -l Filesystem*
	touch stamp-fs-image

$(PGM): $(OBJS)
	$(make-exe)
	arm-rtems4.11-objcopy -O binary -S ${ARCH}/fat_ramdisk.exe ${RTEMS_TFTP}/fat_ramdisk.bin
