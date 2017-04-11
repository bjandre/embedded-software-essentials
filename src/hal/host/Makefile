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

ifeq ($(PLATFORM), $(PLATFORM_ENUM_HOST))
LIB = \
	libesehost.a

SRCS = \
	dma-host.c \
	uart-host.c \
	rtc-host.c

OBJS = $(SRCS:%.c=%.o)

INCLUDES = \
	-I../../util \
	-I../../main \
	-I../../app \
	-I..

else ifeq ($(PLATFORM), $(PLATFORM_ENUM_BBB))
LIB = \
	libesehost.a

SRCS = \
	dma-host.c \
	uart-host.c \
	rtc-host.c

OBJS = $(SRCS:%.c=%.o)

INCLUDES = \
	-I../../util \
	-I../../main \
	-I../../app \
	-I..
endif

include $(MAKERULES)/generic.makefile




