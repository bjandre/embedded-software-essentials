# -*- mode: Makefile -*-
WORKSPACE_PATH := $(shell expr $(PWD) : '\(.*\)/src.*' \| $(PWD))
MAKERULES = $(abspath $(WORKSPACE_PATH)/makerules)

include $(MAKERULES)/config.makefile

# subdirectories that will be built.
#SUBDIRS = \

LIB = libproject1.a

SRCS = project_1.c

OBJS = $(SRCS:%.c=%.o)

INCLUDES = -I../arch -I../util


include $(MAKERULES)/generic.makefile




