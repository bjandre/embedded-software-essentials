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
		$(MAKE) --directory=$@ $(MAKECMDGOALS); \
	fi
	@echo $(HOST_SYSTEM)-$(TARGET_SYSTEM) build complete: $@ : $(shell date)
endif

.PHONY : all
all : $(SUBDIRS)

.PHONY : comile-all
compile-all : $(SUBDIRS)

.PHONY : test
test : $(SUBDIRS)

.PHONY : astyle
astyle : FORCE
	astyle --options=.astylerc --recursive src/*.c src/*.h

.PHONY : clean
clean : $(SUBDIRS)
	@-$(RM) -rf $(EDITOR_FILES) $(BUILD_ARTIFACTS)

FORCE :

