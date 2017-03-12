
# Copyright 2017 Benjamin J. Andre.
# All Rights Reserved.
# 
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

# command line configurable options

release = unset
platform = unset
debug_rules = unset
project = unset

# define the project macro controlling the build code.
strip_project := $(strip $(project))
ifeq ($(strip_project), unset)
  PROJECT := 2
else 
  PROJECT := $(strip_project)
endif

DEFINES += -DPROJECT=$(PROJECT)
