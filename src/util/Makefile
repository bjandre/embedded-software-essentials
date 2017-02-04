# -*- mode: Makefile -*-
WORKSPACE_PATH := $(shell expr $(PWD) : '\(.*\)/src.*' \| $(PWD))
MAKERULES = $(WORKSPACE_PATH)/makerules

include $(MAKERULES)/config.makefile

# subdirectories that will be built.
#SUBDIRS = 

LIB = \
	libutil.a

SRCS = \
	memory.c

OBJS = $(SRCS:%.c=%.o)

#
# macro for executing TARGET in all SUBDIRS
#
ifdef SUBDIRS
.PHONY : $(SUBDIRS)
$(SUBDIRS) : 
	@if [ -d $@ ]; then \
		$(MAKE) --directory=$@ $(MAKECMDGOALS); \
	fi	
	$(BUILD_COMPLETE)
endif	

include $(MAKERULES)/generic.makefile

.PHONY : all
all : $(SUBDIRS) $(LIB)

$(LIB) : $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

.PHONY : test
test : $(SUBDIRS) all




