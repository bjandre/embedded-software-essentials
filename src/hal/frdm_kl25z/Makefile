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
	initialize_frdm_kl25z.c \
	pwm_timer_frdm_kl25z.c \
	profiling_timer_frdm_kl25z.c \
	rtc_frdm_kl25z.c \
	dma_frdm_kl25z.c \
	uart_frdm_kl25z.c \
	spi_frdm_kl25z.c \
	gpio_frdm_kl25z.c

STARTUP_LIB = libfrdm_startup.a
STARTUP_SRCS = system_MKL25Z4.c
STARTUP_OBJS = $(STARTUP_SRCS:%.c=%.o)
ASM_SRCS = startup_MKL25Z4.S
ASM_OBJS = $(ASM_SRCS:%.S=%.o)

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








