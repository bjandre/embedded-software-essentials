#
# configuration for ubuntu 16.04 - BeagleBoneBlack cross compiling
# with arm-linux-gnueabihf
#
ARCH := armv7-a
CPU := cortex-a8

CC = arm-linux-gnueabihf-gcc
CFLAGS = -march=$(ARCH) -mtune=$(CPU) -mfpu=neon \
  -Wl,--print-output-format

AR = arm-linux-gnueabihf-ar

LD = arm-linux-gnueabihf-ld
LDFLAGS = \
  --print-output-format \
  -L/usr/lib/gcc-cross/arm-linux-gnueabihf/5 \
  -L/usr/lib/gcc-cross/arm-linux-gnueabihf/5/../../../../arm-linux-gnueabihf/lib \
  -dynamic-linker /lib/ld-linux-armhf.so.3

LDLIBS = \
  /usr/lib/gcc-cross/arm-linux-gnueabihf/5/../../../../arm-linux-gnueabihf/lib/crt1.o \
  /usr/lib/gcc-cross/arm-linux-gnueabihf/5/../../../../arm-linux-gnueabihf/lib/crti.o \
  /usr/lib/gcc-cross/arm-linux-gnueabihf/5/crtbegin.o \
  -lgcc --as-needed -lgcc_s --no-as-needed -lc -lgcc \
  --as-needed -lgcc_s --no-as-needed \
  /usr/lib/gcc-cross/arm-linux-gnueabihf/5/crtend.o \
  /usr/lib/gcc-cross/arm-linux-gnueabihf/5/../../../../arm-linux-gnueabihf/lib/../lib/crtn.o

SIZE = arm-linux-gnueabihf-size
SIZEFLAGS = --format=sysv -x
