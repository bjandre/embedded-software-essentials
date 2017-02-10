# -*- mode: Makefile -*-

# Makefile for building ECEN 5013 class projects on unix systems
WORKSPACE_PATH := $(PWD)
MAKERULES = $(WORKSPACE_PATH)/makerules

include $(MAKERULES)/config.makefile

# subdirectories that will be built.
SUBDIRS = \
	src

#
# macro for executing TARGET in all SUBDIRS
#
ifdef SUBDIRS
.PHONY : $(SUBDIRS)
$(SUBDIRS) : FORCE
	@if [ -d $@ ]; then \
		$(MAKE) --no-print-directory --directory=$@ $(MAKECMDGOALS); \
	fi
	@echo '$(HOST_SYSTEM)'-'$(TARGET_SYSTEM)' build complete: $@ : $(shell date)
endif

include $(MAKERULES)/generic.makefile

.PHONY : astyle
astyle : FORCE
	astyle --options=.astylerc --recursive src/*.c src/*.h


