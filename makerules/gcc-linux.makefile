
# Copyright 2017 Benjamin J. Andre.
# All Rights Reserved.
# 
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

#
# generic gcc commands for native builds on the host.
#

CC = cc
CFLAGS = $(DEFINE_LOGGER_ALGORITHM)=$(LOGGER_POLLING)

CPPFLAGS = -E

AR = ar

LD = ld

SIZE = size
SIZEFLAGS = --format=sysv -x

OBJDUMP = objdump
OBJDUMP_FLAGS = --disassemble-all --line-numbers --source --full-contents --all-headers

