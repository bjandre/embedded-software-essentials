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
	libapp.a

SRCS = \
	nrf24l01.c \
	post-nrf24l01.c \
	memory-profile.c \
	analyze-data.c \
	heartbeat.c \
	profiler.c \
	post-profiler.c \
	message.c \
	command.c \
	logger.c \
	log-item.c

INCLUDES = -I. -I../util -I../hal -I../main

ifeq ($(PLATFORM), $(PLATFORM_ENUM_HOST))
  INCLUDES += -I../hal/host
else ifeq ($(PLATFORM), $(PLATFORM_ENUM_BBB))
  INCLUDES += -I../hal/beagle-bone-black
else ifeq ($(PLATFORM), $(PLATFORM_ENUM_FRDM))
  INCLUDES += -I../hal/frdm-kl25z
endif

OBJS = $(SRCS:%.c=%.o)


INCLUDES += -I$(CMOCKA_INCLUDE_DIR)

TEST_SRCS = \
	test_profiler.c \
	test_app.c

TEST_OBJS = $(TEST_SRCS:%.c=%.o)
TEST_EXE = test_app.out
TEST_DEPEND_LIBS = $(CMOCKA_LIBRARY) 

#
# macro for executing TARGET in all SUBDIRS
#

include $(MAKERULES)/generic.makefile

$(TEST_EXE) : profiler.o $(TEST_OBJS)
	$(CC) $(CFLAGS) $(CMOCKA_INCLUDES) -O0 -g -U NDEBUG -o $@ $^ $(TEST_DEPEND_LIBS)





