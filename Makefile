# -*- mode: Makefile -*-

# Copyright 2017 Benjamin J. Andre.
# All Rights Reserved.
# 
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.


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
	astyle --options=.astylerc --recursive 'src/*.c' 'src/*.h'

.PHONY : archive
archive : FORCE
	tar --exclude .git/ -c -v -f andre-ecen5013-project$(PROJECT).tgz .

.PHONY : doc
doc : FORCE
	$(DOXYGEN) doc/Doxyfile

.PHONY : clean-doc
clean-doc : FORCE
	@-$(RM) -rf $(DOC_HTML) $(DOC_LATEX)
