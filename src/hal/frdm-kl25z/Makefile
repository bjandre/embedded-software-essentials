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

ifeq ($(PLATFORM), $(PLATFORM_ENUM_FRDM))
LIB = \
	libesefrdm.a

SRCS = \
	initialize-frdm-kl25z.c \
	dma-frdm-kl25z.c \
	post-dma-frdm-kl25z.c \
	uart-frdm-kl25z.c \
	spi-frdm-kl25z.c \
	interrupts-frdm-kl25z.c \
	gpio-frdm-kl25z.c

STARTUP_LIB = libfrdm_startup.a
STARTUP_SRCS = system_MKL25Z4.c
STARTUP_OBJS = $(STARTUP_SRCS:%.c=%.o)
ASM_SRCS = startup_MKL25Z4.S
ASM_OBJS = $(ASM_SRCS:%.S=%.o)

OBJS = $(SRCS:%.c=%.o)

INCLUDES = \
	-I../../main \
	-I../../util \
	-I.. \
	-I.

endif

include $(MAKERULES)/generic.makefile








