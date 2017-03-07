# -*- mode: Makefile -*-
WORKSPACE_PATH := $(shell expr $(PWD) : '\(.*\)/src.*' \| $(PWD))
MAKERULES = $(abspath $(WORKSPACE_PATH)/makerules)

include $(MAKERULES)/config.makefile

ifeq ($(PLATFORM), $(PLATFORM_ENUM_FRDM))
LIB = \
	libesefrdm.a

SRCS = \
	uart-frdm-kl25z.c

STARTUP_LIB = libfrdm_startup.a
STARTUP_SRCS = system_MKL25Z4.c
STARTUP_OBJS = $(STARTUP_SRCS:%.c=%.o)
ASM_SRCS = startup_MKL25Z4.S
ASM_OBJS = $(ASM_SRCS:%.S=%.o)

OBJS = $(SRCS:%.c=%.o)

INCLUDES = \
	-I../../util \
	-I..

endif

include $(MAKERULES)/generic.makefile








