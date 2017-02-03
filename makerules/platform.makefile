# command line configurable options
debug = unset
platform = unset

# setup options based on command line options and defaults.
ifeq ($(platform), unset)
  SYSTEM_TYPE = $(shell uname -s)
else
  ifeq ($(platform), host)
    PLATFORM = host
    SYSTEM_TYPE = $(shell uname -s)
  else
    ifeq ($(platform), bbb)
      PLATFORM = bbb
      SYSTEM_TYPE = bbb-cross
    else
      ifeq ($(platform), frdm)
        PLATFORM = frdm
	SYSTEM_TYPE = bare
      endif
    endif
  endif
endif

# native compilation for os x, linux
NATIVE_CC = cc
NATIVE_CFLAGS = --std=c99 -Wall -g
NATIVE_AR = ar
NATIVE_LD = ld

# arm-linux-gnueabihf cross compiler for BeagleBone Black
ALG_X_CC = arm-linux-gnueabihf-gcc
ALG_X_CFLAGS = --std=c99 -Wall -g -march=armv7-a -mtune=cortex-a8 -mfpu=neon -Wl,--print-output-format \

ALG_X_AR = arm-linux-gnueabihf-ar
ALG_X_LD = arm-linux-gnueabihf-ld
ALG_X_LDFLAGS = \
	--print-output-format \
	-L/usr/lib/gcc-cross/arm-linux-gnueabihf/5 \
	-L/usr/lib/gcc-cross/arm-linux-gnueabihf/5/../../../../arm-linux-gnueabihf/lib \
	-dynamic-linker /lib/ld-linux-armhf.so.3
ALG_X_LDLIBS = \
	/usr/lib/gcc-cross/arm-linux-gnueabihf/5/../../../../arm-linux-gnueabihf/lib/crt1.o \
	/usr/lib/gcc-cross/arm-linux-gnueabihf/5/../../../../arm-linux-gnueabihf/lib/crti.o \
	/usr/lib/gcc-cross/arm-linux-gnueabihf/5/crtbegin.o \
	-lgcc --as-needed -lgcc_s --no-as-needed -lc -lgcc \
	--as-needed -lgcc_s --no-as-needed \
	/usr/lib/gcc-cross/arm-linux-gnueabihf/5/crtend.o \
	/usr/lib/gcc-cross/arm-linux-gnueabihf/5/../../../../arm-linux-gnueabihf/lib/../lib/crtn.o

ifeq ($(SYSTEM_TYPE), Darwin)
  CC = $(NATIVE_CC)
  CFLAGS = $(NATIVE_CFLAGS)
  AR = $(NATIVE_AR)
  LD = $(NATIVE_LD)
  LDFLAGS = -macosx_version_min 10.11 -lSystem
else
  ifeq ($(SYSTEM_TYPE), Linux)
    CC = $(NATIVE_CC)
    CFLAGS = $(NATIVE_CFLAGS)
    AR = $(NATIVE_AR)
    LD = $(NATIVE_LD)
    LDFLAGS = \
	-L/usr/lib/gcc/x86_64-linux-gnu/5 \
	-L/usr/lib/gcc/x86_64-linux-gnu/5/../../../x86_64-linux-gnu \
	-L/usr/lib/gcc/x86_64-linux-gnu/5/../../../../lib \
	-L/lib/x86_64-linux-gnu -L/lib/../lib\
	-L/usr/lib/x86_64-linux-gnu \
	-L/usr/lib/../lib \
	-L/usr/lib/gcc/x86_64-linux-gnu/5/../../.. \
	-dynamic-linker /lib64/ld-linux-x86-64.so.2
    LDLIBS = \
	/usr/lib/gcc/x86_64-linux-gnu/5/../../../x86_64-linux-gnu/crt1.o \
	/usr/lib/gcc/x86_64-linux-gnu/5/../../../x86_64-linux-gnu/crti.o \
	/usr/lib/gcc/x86_64-linux-gnu/5/crtbegin.o \
	-lgcc --as-needed -lgcc_s --no-as-needed -lc -lgcc --as-needed -lgcc_s --no-as-needed \
	/usr/lib/gcc/x86_64-linux-gnu/5/crtend.o \
	/usr/lib/gcc/x86_64-linux-gnu/5/../../../x86_64-linux-gnu/crtn.o
  else
    ifeq ($(SYSTEM_TYPE), bbb-cross)
      CC = $(ALG_X_CC)
      CFLAGS = $(ALG_X_CFLAGS)
      AR = $(ALG_X_AR)
      LD = $(ALG_X_LD)
      LDFLAGS = $(ALG_X_LDFLAGS)
      LDLIBS = $(ALG_X_LDLIBS)
    endif
  endif
endif

ifeq ($(debug), unset)
  DEBUG = 1
else
  DEBUG = $(debug)
endif
