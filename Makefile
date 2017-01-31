# -*- mode: Makefile -*-

# Makefile for building ECEN 5013 class projects on unix systems
include globals.makefile


# command line configurable options
debug = unset
platform = unset

# setup options based on command line options and defaults.
ifeq ($(platform), unset)
  PLATFORM = $(shell uname -s)
else
  ifeq ($(platform), host)
    PLATFORM = host
  else
    ifeq ($(platform), bbb)
      PLATFORM = bbb
    else
      ifeq ($(platform), frdm)
        PLATFORM = frdm
      endif
    endif
  endif
endif

ifeq ($(debug), unset)
  DEBUG = 1
else
  DEBUG = $(debug)
endif

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

.PHONY : clean
clean : $(SUBDIRS)
	-rm -rf $(EDITOR_FILES) $(BUILD_ARTIFACTS)

FORCE :

