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
	libhal.a

SRCS = \
	memory_dma.c \
	post_dma.c \
	hal.c \
	gpio.c \
	communication_peripheral_generic.c \
	communication_peripheral_factory.c \
	spi_peripheral_factory.c


INCLUDES = -I. -I../util -I../main -I../app

ifeq ($(PLATFORM), $(PLATFORM_ENUM_HOST))
  INCLUDES += -Ihost
else ifeq ($(PLATFORM), $(PLATFORM_ENUM_BBB))
  INCLUDES += -Ibeaglebone_black
else ifeq ($(PLATFORM), $(PLATFORM_ENUM_FRDM))
  INCLUDES += -Ifrdm_kl25z
endif

OBJS = $(SRCS:%.c=%.o)



# subdirectories that will be built.
SUBDIRS = \
	host \
	frdm_kl25z \
	beaglebone_black 

#
# macro for executing TARGET in all SUBDIRS
#

include $(MAKERULES)/generic.makefile






