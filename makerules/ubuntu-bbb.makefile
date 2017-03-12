
# Copyright 2017 Benjamin J. Andre.
# All Rights Reserved.
# 
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

#
# configuration for ubuntu 16.04 - BeagleBoneBlack cross compiling
# with arm-linux-gnueabihf
#
ARCH := armv7-a
CPU := cortex-a8

TOOLS_PREFIX := arm-linux-gnueabihf

CC = $(TOOLS_PREFIX)-gcc
CFLAGS = -march=$(ARCH) -mtune=$(CPU) -mfpu=neon \
  -Wl,--print-output-format

AR = $(TOOLS_PREFIX)-ar

LD = $(TOOLS_PREFIX)-ld
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

SIZE = $(TOOLS_PREFIX)-size
SIZEFLAGS = --format=sysv -x

OBJDUMP = $(TOOLS_PREFIX)-objdump
OBJDUMP_FLAGS = --disassemble-all --line-numbers --source --full-contents --all-headers

#
# info for automatically transfering cross compiled blobs to the
# target. SSH_HOST is the ssh config host alias or the full ssh
# user@machine:port necessary for ssh/sft/scp to work.
#

SCP := scp
SSH_HOST := bbb
CX_PREFIX := x
