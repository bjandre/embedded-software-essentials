
# Copyright 2017 Benjamin J. Andre.
# All Rights Reserved.
# 
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

# native compilation for os x, linux
NATIVE_CC = cc
NATIVE_CFLAGS = --std=c99 -Wall -g

# arm-linux-gnueabi cross compiler
ALG_X_CC = arm-linux-gnueabihf-gcc
ALG_X_CFLAGS = --std=c99 -Wall -g -march=armv7-a -mtune=cortex-a8 -mfpu=neon

platform = unset
ifeq ($(platform), unset)
  CC = $(NATIVE_CC)
  CFLAGS = $(NATIVE_CFLAGS)
else
  ifeq ($(platform), bbb)
    CC = $(ALG_X_CC)
    CFLAGS = $(ALG_X_CFLAGS)
  endif
endif

#------------------------------------------
HELLO_EXE = hello.out

HELLO_SRCS = hello.c

HELLO_OBJS = $(HELLO_SRCS:%.c=%.o)

#------------------------------------------
REVERSE_EXE = reverse.out

REVERSE_SRCS = reverse.c reverse-main.c

REVERSE_OBJS = $(REVERSE_SRCS:%.c=%.o)

#------------------------------------------
SIZEOF_EXE = sizeof.out

SIZEOF_SRCS = sizeof.c

SIZEOF_OBJS = $(SIZEOF_SRCS:%.c=%.o)

#------------------------------------------
%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY : all
all : $(HELLO_EXE) $(REVERSE_EXE) $(SIZEOF_EXE)

$(HELLO_EXE) : $(HELLO_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(REVERSE_EXE) : $(REVERSE_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(SIZEOF_EXE) : $(SIZEOF_OBJS)
	$(CC) $(CFLAGS) -o $@ $^


.PHONY : clean
clean : FORCE
	-rm -rf *.o *~ $(HELLO_EXE) $(REVERSE_EXE) $(SIZEOF_EXE)

FORCE :


