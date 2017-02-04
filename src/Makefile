# -*- mode: Makefile -*-
WORKSPACE_PATH := $(shell expr $(PWD) : '\(.*\)/src.*' \| $(PWD))
MAKERULES = $(abspath $(WORKSPACE_PATH)/makerules)

include $(MAKERULES)/config.makefile

# subdirectories that will be built in order.
SUBDIRS = \
	arch \
	util \
	project-1 \
	main

#
# macro for executing TARGET in all SUBDIRS
#
ifdef SUBDIRS
.PHONY : $(SUBDIRS)
$(SUBDIRS) : FORCE
	@if [ -d $@ ]; then \
		$(MAKE) --directory=$@ $(MAKECMDGOALS); \
	fi
	$(BUILD_COMPLETE)
endif	

.PHONY : all
all : $(SUBDIRS)

.PHONY : comile-all
compile-all : $(SUBDIRS)

.PHONY : test
test : $(SUBDIRS)

.PHONY : clean
clean : $(SUBDIRS)
	@-$(RM) -rf $(EDITOR_FILES) $(BUILD_ARTIFACTS)


FORCE : 
