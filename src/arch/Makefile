# -*- mode: Makefile -*-
WORKSPACE_PATH := $(shell expr $(PWD) : '\(.*\)/src.*' \| $(PWD))
MAKERULES = $(abspath $(WORKSPACE_PATH)/makerules)

include $(MAKERULES)/config.makefile

#LIB = \
#	libarch.a

#SRCS = frdm-kl25z.c

#OBJS = $(SRCS:%.c=%.o)

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






