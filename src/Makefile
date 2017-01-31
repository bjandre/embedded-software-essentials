# -*- mode: Makefile -*-
include ../globals.makefile

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
	@echo Build complete: $@ : $(shell date)
endif	

.PHONY : all
all : $(SUBDIRS)

.PHONY : comile-all
compile-all : $(SUBDIRS)

.PHONY : test
test : $(SUBDIRS)

.PHONY : clean
clean : $(SUBDIRS)
	-rm -rf $(EDITOR_FILES) $(BUILD_ARTIFACTS)


FORCE : 
