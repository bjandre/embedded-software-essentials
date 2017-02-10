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

#
# macro for executing TARGET in all SUBDIRS
#
ifdef SUBDIRS
.PHONY : $(SUBDIRS)
$(SUBDIRS) : 
	@if [ -d $@ ]; then \
		$(MAKE) --directory=$@ $(TARGET); \
	fi	
	$(BUILD_COMPLETE)
endif	

include $(MAKERULES)/generic.makefile




