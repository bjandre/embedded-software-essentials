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
	libapp.a

SRCS = \
	logger.c \
	log-item.c

INCLUDES = -I. -I../util -I../hal -I../main

ifeq ($(PLATFORM), $(PLATFORM_ENUM_HOST))
  INCLUDES += -I../host
else ifeq ($(PLATFORM), $(PLATFORM_ENUM_BBB))
  INCLUDES += -I../host
else ifeq ($(PLATFORM), $(PLATFORM_ENUM_FRDM))
  INCLUDES += -I../hal/frdm-kl25z
endif

OBJS = $(SRCS:%.c=%.o)


#
# macro for executing TARGET in all SUBDIRS
#

include $(MAKERULES)/generic.makefile






