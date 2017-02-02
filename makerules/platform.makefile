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
