
# Copyright 2017 Benjamin J. Andre.
# All Rights Reserved.
# 
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

#
# configuration for native BeagleBone Black builds
#
include $(MAKERULES)/gcc-linux.makefile

LDFLAGS = \
	-L/usr/lib/gcc/arm-linux-gnueabihf/4.6 \
	-L/usr/lib/gcc/arm-linux-gnueabihf/4.6/../../../arm-linux-gnueabihf \
	-L/usr/lib/gcc/arm-linux-gnueabihf/4.6/../../.. \
	-L/lib/arm-linux-gnueabihf \
	-L/usr/lib/arm-linux-gnueabihf \
	-dynamic-linker /lib/ld-linux-armhf.so.3 \
	-Map=$*.map

LDLIBS = /usr/lib/gcc/arm-linux-gnueabihf/4.6/../../../arm-linux-gnueabihf/crt1.o \
	/usr/lib/gcc/arm-linux-gnueabihf/4.6/../../../arm-linux-gnueabihf/crti.o \
	/usr/lib/gcc/arm-linux-gnueabihf/4.6/crtbegin.o \
	-lgcc --as-needed -lgcc_s --no-as-needed -lc -lgcc --as-needed -lgcc_s --no-as-needed \
	/usr/lib/gcc/arm-linux-gnueabihf/4.6/crtend.o \
	/usr/lib/gcc/arm-linux-gnueabihf/4.6/../../../arm-linux-gnueabihf/crtn.o

