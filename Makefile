# -*- mode: Makefile -*-

# Makefile for building ECEN 5013 class projects on unix systems
WORKSPACE_PATH := $(PWD)
MAKERULES = $(WORKSPACE_PATH)/makerules

include $(MAKERULES)/config.makefile

# subdirectories that will be built.
SUBDIRS = \
	src


include $(MAKERULES)/3rd-party.makefile

include $(MAKERULES)/generic.makefile

.PHONY : astyle
astyle : FORCE
	astyle --options=.astylerc --recursive src/*.c src/*.h

.PHONY : archive
archive : FORCE
	tar --exclude .git/ -c -v -f andre-ecen5013-project$(PROJECT).tgz .

