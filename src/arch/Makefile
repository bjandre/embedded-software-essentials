# -*- mode: Makefile -*-
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

ifeq ($(PLATFORM), $(PLATFORM_ENUM_HOST))
  SRCS += $(SRCS_HOST)
else ifeq ($(PLATFORM), $(PLATFORM_ENUM_BBB))
  SRCS += $(SRCS_HOST)
else ifeq ($(PLATFORM), $(PLATFORM_ENUM_FRDM))
  SRCS += $(SRCS_FRDM)
  STARTUP_LIB = libfrdm.a
  STARTUP_SRCS = system_MKL25Z4.c
  STARTUP_OBJS = $(STARTUP_SRCS:%.c=%.o)
  ASM_SRCS = startup_MKL25Z4.S
  ASM_OBJS = $(ASM_SRCS:%.S=%.o)
endif

OBJS = $(SRCS:%.c=%.o)

INCLUDES = -I../util


# subdirectories that will be built.
SUBDIRS = 
#	beagle-bone-black \
#	freedom \
#	host

#
# macro for executing TARGET in all SUBDIRS
#

include $(MAKERULES)/generic.makefile






