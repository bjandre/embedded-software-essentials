
# Copyright 2017 Benjamin J. Andre.
# All Rights Reserved.
# 
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

#
# configuration for native ubuntu builds
#
include $(MAKERULES)/gcc-linux.makefile

LDFLAGS = \
	-L/usr/lib/gcc/x86_64-linux-gnu/5 \
	-L/usr/lib/gcc/x86_64-linux-gnu/5/../../../x86_64-linux-gnu \
	-L/usr/lib/gcc/x86_64-linux-gnu/5/../../../../lib \
	-L/lib/x86_64-linux-gnu -L/lib/../lib\
	-L/usr/lib/x86_64-linux-gnu \
	-L/usr/lib/../lib \
	-L/usr/lib/gcc/x86_64-linux-gnu/5/../../.. \
	-dynamic-linker /lib64/ld-linux-x86-64.so.2 \
	-Map=$*.map
LDLIBS = \
	/usr/lib/gcc/x86_64-linux-gnu/5/../../../x86_64-linux-gnu/crt1.o \
	/usr/lib/gcc/x86_64-linux-gnu/5/../../../x86_64-linux-gnu/crti.o \
	/usr/lib/gcc/x86_64-linux-gnu/5/crtbegin.o \
	-lgcc --as-needed -lgcc_s --no-as-needed -lc -lgcc --as-needed -lgcc_s --no-as-needed \
	/usr/lib/gcc/x86_64-linux-gnu/5/crtend.o \
	/usr/lib/gcc/x86_64-linux-gnu/5/../../../x86_64-linux-gnu/crtn.o

