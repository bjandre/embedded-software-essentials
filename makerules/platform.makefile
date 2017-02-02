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
      SYSTEM_TYPE = Linux
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

# arm-linux-gnueabi cross compiler
ALG_X_CC = arm-linux-gnueabihf-gcc
ALG_X_CFLAGS = --std=c99 -Wall -g -march=armv7-a -mtune=cortex-a8 -mfpu=neon

ifeq ($(SYSTEM_TYPE), Darwin)
  CC = $(NATIVE_CC)
  CFLAGS = $(NATIVE_CFLAGS) 
  LDFLAGS = -macosx_version_min 10.11 -lSystem
else
  ifeq ($(SYSTEM_TYPE), Linux)
    CC = $(NATIVE_CC)
    CFLAGS = $(NATIVE_CFLAGS)
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
    ifeq ($(platform), bbb)
      CC = $(ALG_X_CC)
      CFLAGS = $(ALG_X_CFLAGS)
    endif
  endif
endif

ifeq ($(debug), unset)
  DEBUG = 1
else
  DEBUG = $(debug)
endif
