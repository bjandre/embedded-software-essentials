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
