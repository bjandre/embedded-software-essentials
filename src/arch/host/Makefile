# -*- mode: Makefile -*-
WORKSPACE_PATH := $(shell expr $(PWD) : '\(.*\)/src.*' \| $(PWD))
MAKERULES = $(abspath $(WORKSPACE_PATH)/makerules)

include $(MAKERULES)/config.makefile

ifeq ($(PLATFORM), $(PLATFORM_ENUM_HOST))
LIB = \
	libesehost.a

SRCS = \
	uart-host.c

OBJS = $(SRCS:%.c=%.o)

INCLUDES = \
	-I../util \
	-I..


endif

include $(MAKERULES)/generic.makefile




