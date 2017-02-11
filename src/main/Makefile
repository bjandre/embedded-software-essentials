# -*- mode: Makefile -*-
WORKSPACE_PATH := $(shell expr $(PWD) : '\(.*\)/src.*' \| $(PWD))
MAKERULES = $(WORKSPACE_PATH)/makerules

include $(MAKERULES)/config.makefile

EXE = main.out

SRCS = \
	main.c

OBJS = $(SRCS:%.c=%.o)

LIBS = \
	-L../util -lutil

DEPEND_LIBS = \
	../project-1/libproject1.a \
	../util/libutil.a

INCLUDES = \
	-I../util \
	-I../project-1

TEST_EXE = $(EXE)

include $(MAKERULES)/generic.makefile

