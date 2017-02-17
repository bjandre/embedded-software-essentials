#
# configuration for ubuntu 16.04 - Freedom KL25z development board cross compiling
# with arm-none-eabi tool chain
#
ARCH := armv6-m
CPU := cortex-m0plus

TOOLS_PREFIX := arm-none-eabi

CC = $(TOOLS_PREFIX)-gcc
CFLAGS = -march=$(ARCH) -mtune=$(CPU) -mthumb \
  -Wl,--print-output-format

AR = $(TOOLS_PREFIX)-ar

LD = $(TOOLS_PREFIX)-ld
LDFLAGS = \
  --print-output-format \
	-L/usr/lib/gcc/arm-none-eabi/4.9.3/armv6-m \
	-L/usr/lib/gcc/arm-none-eabi/4.9.3/../../../arm-none-eabi/lib/armv6-m \
	-L/usr/lib/gcc/arm-none-eabi/4.9.3 \
	-L/usr/lib/gcc/arm-none-eabi/4.9.3/../../../arm-none-eabi/lib

LDLIBS = \
	/usr/lib/gcc/arm-none-eabi/4.9.3/armv6-m/crti.o \
	/usr/lib/gcc/arm-none-eabi/4.9.3/armv6-m/crtbegin.o \
	/usr/lib/gcc/arm-none-eabi/4.9.3/../../../arm-none-eabi/lib/armv6-m/crt0.o \
	--start-group -lgcc -lc -lrdimon --end-group \
	/usr/lib/gcc/arm-none-eabi/4.9.3/armv6-m/crtend.o \
	/usr/lib/gcc/arm-none-eabi/4.9.3/armv6-m/crtn.o


SIZE = $(TOOLS_PREFIX)-size
SIZEFLAGS = --format=sysv -x

OBJDUMP = $(TOOLS_PREFIX)-objdump
OBJDUMP_FLAGS = --disassemble-all --line-numbers --source --full-contents --all-headers
