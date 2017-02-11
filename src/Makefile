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


include $(MAKERULES)/generic.makefile


