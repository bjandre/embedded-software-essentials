# -*- mode: Makefile -*-

# Copyright 2017 Benjamin J. Andre.
# All Rights Reserved.
# 
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

WORKSPACE_PATH := $(shell expr $(PWD) : '\(.*\)/src.*' \| $(PWD))
MAKERULES = $(WORKSPACE_PATH)/makerules

include $(MAKERULES)/config.makefile

EXE = $(EXE_BASE).$(EXE_EXTENSION)

BIN = $(EXE:%.$(EXE_EXTENSION)=%.$(BIN_EXTENSION))

SRCS = \
	main.c \
	async-global.c \
	debug-uart-data.c \
	post.c

OBJS = $(SRCS:%.c=%.o)

INCLUDES = \
	-I. \
	-I../app \
	-I../hal \
	-I../util \
	-I../project-1

DEPEND_LIBS += \
	../project-1/libproject1.a \
	../app/libapp.a \
	../hal/libhal.a

LIBS += \
	-L../project-1 -lproject1 \
	-L../app -lapp \
	-L../hal -lhal

ifeq ($(PLATFORM), $(PLATFORM_ENUM_FRDM))
  INCLUDES += \
	-I../hal/frdm-kl25z
  DEPEND_STARTUP += \
	../hal/frdm-kl25z/libfrdm_startup.a
  DEPEND_LIBS += \
	../hal/frdm-kl25z/libesefrdm.a 
  LIBS += \
	-L../hal/frdm-kl25z -lesefrdm
else ifeq ($(PLATFORM), $(PLATFORM_ENUM_HOST))
  INCLUDES += \
	-I../hal/host
  DEPEND_LIBS += \
	../hal/host/libesehost.a
  LIBS += \
	-L../hal/host -lesehost
else ifeq ($(PLATFORM), $(PLATFORM_ENUM_BBB))
  INCLUDES += \
	-I../hal/beagle-bone-black
  DEPEND_LIBS += \
	../hal/beagle-bone-black/libesebbb.a
  LIBS += \
	-L../hal/beagle-bone-black -lesebbb
endif

LIBS += \
	-L../util -lutil

DEPEND_LIBS += \
	../util/libutil.a

TEST_EXE = $(EXE)

TEST_EXE_ARGS = < console-input-cmd-help.hex > output.txt; \
	rm hr-output.txt; \
	./ese-process-logs.py < output.txt > hr-output.txt; \
	./ese-process-logs.py < baseline.txt > hr-baseline.txt; \
	diff baseline.txt output.txt; \
	cat hr-output.txt; \
	diff --text hr-baseline.txt hr-output.txt

include $(MAKERULES)/generic.makefile

.PHONY : bin
bin : $(BIN)

$(BIN) : $(EXE)
ifdef BARE_METAL
	$(OBJCOPY) $(OBJCOPY_FLAGS) $< $@
#	$(CP) $@ $(OPENSDA_FLASH_MSD)/$@
endif
