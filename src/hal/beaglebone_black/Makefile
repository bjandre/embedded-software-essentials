# -*- mode: Makefile -*-

# Copyright 2017 Benjamin J. Andre.
# All Rights Reserved.
# 
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

WORKSPACE_PATH := $(shell expr $(PWD) : '\(.*\)/src.*' \| $(PWD))
MAKERULES = $(abspath $(WORKSPACE_PATH)/makerules)

include $(MAKERULES)/config.makefile

ifeq ($(PLATFORM), $(PLATFORM_ENUM_BBB))
LIB = \
	libesebbb.a

SRCS = \
	dma_bbb.c \
	rtc_bbb.c \
	spi_bbb.c \
	uart_bbb.c

OBJS = $(SRCS:%.c=%.o)

INCLUDES = \
	-I../../main \
	-I../../app \
	-I../../util \
	-I.. \
	-I.

EPOCH_TIME := $(shell date +%s)
CFLAGS += -DEPOCH_TIME=$(EPOCH_TIME)

endif

include $(MAKERULES)/generic.makefile








