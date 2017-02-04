#
# figure out what type of host system we are building on
#
ifeq ($(shell uname -s), Linux)
  HOST_SYSTEM = $(shell lsb_release -ds)
else
  HOST_SYSTEM = $(shell uname -s)
endif
ifneq ($(debug_rules), unset)
  $(info Host system is $(HOST_SYSTEM))
endif

#
# figure out the target system we are building for
#
#$(info user arg platform is $(platform))
strip_platform = $(strip $(platform))
ifeq ($(strip_platform), $(filter host unset '', $(strip_platform)))
  # not specified, default to host
  TARGET_SYSTEM = $(HOST_SYSTEM)
else ifeq ($(platform), bbb)
  TARGET_SYSTEM = bbb
else ifeq ($(platform), frdm)
  TARGET_SYSTEM = frdm
else
  $(error Can not identify target system!)
endif
ifneq ($(debug_rules), unset)
  $(info Target system is $(TARGET_SYSTEM))
endif

#
# load host-target specific configuration
#
ifeq ($(HOST_SYSTEM), $(TARGET_SYSTEM))
  # build for the host system
  ifeq ("$(HOST_SYSTEM)", "Darwin")
    include $(MAKERULES)/darwin-darwin.makefile
  else
    ifeq ("$(HOST_SYSTEM)", "Ubuntu 16.04.1 LTS")
      include $(MAKERULES)/ubuntu-ubuntu.makefile
    else
      ifeq ("$(HOST_SYSTEM)", "Debian GNU/Linux 7.9 (wheezy)")
        include $(MAKERULES)/bbb-bbb.makefile
      else
        $(error Unsupported host system : "$(HOST_SYSTEM)")
      endif
    endif
  endif
else
  # cross compiling
  ifeq ($(HOST_SYSTEM), Ubuntu 16.04.1 LTS)
    ifeq ($(TARGET_SYSTEM), bbb)
      include $(MAKERULES)/ubuntu-bbb.makefile
    else
      $(error Unsupported ubuntu cross compile : "$(TARGET_SYSTEM)")
    endif
  else
    $(error Unsupported host "$(HOST_SYSTEM)" cross compile to "$(TARGET_SYSTEM)")
  endif
endif

#
# now add the generic compiler options
#

# compile unoptimized debug by default. specific release flags will be
# added depending on the host and target architectures
ifeq ($(release), unset)
  RELEASE_CFLAGS = -g -O0
else
  RELEASE_CFLAGS = 
endif

GENERAL_CFLAGS = --std=c99 -Wall $(RELEASE_FLAGS)
CFLAGS += $(GENERAL_CFLAGS)
CPPFLAGS = -E
DEPENDS_FLAGS = -MT $@ -MMD -MP -MF $(DEPENDS_DIR)/$*.Td

