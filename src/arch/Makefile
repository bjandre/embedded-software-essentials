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

LIB = \
	libarch.a

SRCS = \
	uart.c \
	logger.c

SRCS_HOST = \
	uart-host.c

SRCS_FRDM = \
	uart-frdm-kl25z.c

INCLUDES = -I. -I../util

ifeq ($(PLATFORM), $(PLATFORM_ENUM_HOST))
  INCLUDES += -Ihost
else ifeq ($(PLATFORM), $(PLATFORM_ENUM_BBB))
  INCLUDES += -Ihost
else ifeq ($(PLATFORM), $(PLATFORM_ENUM_FRDM))
  INCLUDES += -Ifrdm-kl25z
endif

OBJS = $(SRCS:%.c=%.o)



# subdirectories that will be built.
SUBDIRS = \
	host \
	frdm-kl25z
#	beagle-bone-black \

#
# macro for executing TARGET in all SUBDIRS
#

include $(MAKERULES)/generic.makefile






