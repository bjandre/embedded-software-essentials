# -*- mode: Makefile -*-

# Makefile for building ECEN 5013 class projects on unix systems
include makerules/globals.makefile
include makerules/platform.makefile

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
endif
	@echo $(PLATFORM) build complete: $@ : $(shell date)

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
	-rm -rf $(EDITOR_FILES) $(BUILD_ARTIFACTS)

FORCE :

