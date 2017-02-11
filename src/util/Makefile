# -*- mode: Makefile -*-
WORKSPACE_PATH := $(shell expr $(PWD) : '\(.*\)/src.*' \| $(PWD))
MAKERULES = $(WORKSPACE_PATH)/makerules

include $(MAKERULES)/config.makefile

# subdirectories that will be built.
#SUBDIRS = 

LIB = \
	libutil.a

SRCS = \
	memory.c \
	data.c

OBJS = $(SRCS:%.c=%.o)

INCLUDES = -I../arch

TEST_SRCS = test_utils.c
TEST_OBJS = $(TEST_SRCS:%.c=%.o)
TEST_EXE = test_utils.out

include $(MAKERULES)/generic.makefile

$(TEST_EXE) : $(TEST_OBJS) $(OBJS)
	$(CC) $(CFLAGS) -O0 -g -U NDEBUG -o $@ $^


