# all the logic to setup platform specific configuration for the
# current host-target combination

SYS_LINUX := Linux
SYS_DARWIN := Darwin
SYS_UBUNTU := Ubuntu 16.04.1 LTS
SYS_WHEEZY := Debian GNU/Linux 7.9 (wheezy)
PLATFORM_HOST := host
PLATFORM_BBB := bbb
PLATFORM_FRDM := frdm

#
# figure out what type of host system we are building on
#
ifeq ($(shell uname -s), $(SYS_LINUX))
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
ifneq ($(debug_rules), unset)
  $(info Commandline platform is "$(platform)")
endif
strip_platform = $(strip $(platform))
ifeq ($(strip_platform), $(filter $(PLATFORM_HOST) unset '', $(strip_platform)))
  # not specified, default to host
  TARGET_SYSTEM = $(HOST_SYSTEM)
  PLATFORM = $(PLATFORM_HOST)
else ifeq ($(platform), $(PLATFORM_BBB))
  TARGET_SYSTEM = $(PLATFORM_BBB)
  PLATFORM = $(PLATFORM_BBB)
else ifeq ($(platform), $(PLATFORM_FRDM))
  TARGET_SYSTEM = $(PLATFORM_FRDM)
  PLATFORM = $(PLATFORM_FRDM)
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
  ifeq ("$(HOST_SYSTEM)", "$(SYS_DARWIN)")
    include $(MAKERULES)/darwin-darwin.makefile
  else ifeq ("$(HOST_SYSTEM)", "$(SYS_UBUNTU)")
    include $(MAKERULES)/ubuntu-ubuntu.makefile
  else ifeq ("$(HOST_SYSTEM)", "$(SYS_WHEEZY)")
    include $(MAKERULES)/bbb-bbb.makefile
  else
    $(error Unsupported host system : "$(HOST_SYSTEM)")
  endif
else
  # cross compiling
  ifeq ($(HOST_SYSTEM), $(SYS_DARWIN))
    ifeq ($(TARGET_SYSTEM), $(PLATFORM_FRDM))
      include $(MAKERULES)/darwin-frdm.makefile
    else
      $(error Unsupported darwin cross compile : "$(TARGET_SYSTEM)")
    endif
  else ifeq ($(HOST_SYSTEM), $(SYS_UBUNTU))
    ifeq ($(TARGET_SYSTEM), $(PLATFORM_BBB))
      include $(MAKERULES)/ubuntu-bbb.makefile
    else ifeq ($(TARGET_SYSTEM), $(PLATFORM_FRDM))
      include $(MAKERULES)/ubuntu-frdm.makefile
    else
      $(error Unsupported ubuntu cross compile : "$(TARGET_SYSTEM)")
    endif
  else
    $(error Unsupported host "$(HOST_SYSTEM)" cross compile to "$(TARGET_SYSTEM)")
  endif
endif

#
# now add the generic c compiler options
#

# compile unoptimized debug by default. specific release flags will be
# added depending on the host and target architectures
ifeq ($(release), $(filter unset 0 '', $(release)))
  RELEASE_CFLAGS = -g -O0 -UNDEBUG
#  RELEASE_CFLAGS = -DNDEBUG
else
  RELEASE_CFLAGS = -Os -DNDEBUG
endif

# Defines is used elsewhere, e.g. adding the PROJECT macro. But could
# alse be amended here.

DEFINES += -DPLATFORM=$(PLATFORM)

GENERAL_CFLAGS = --std=c99 -Wall -Werror

CFLAGS += $(RELEASE_CFLAGS) $(DEFINES) $(GENERAL_CFLAGS)

# flags for generating assembly files from c.
C_ASM_FLAGS = -S

# flags for writing preprocessed output
CPPFLAGS = -E

# flags to automatically generate dependency information
DEPENDS_FLAGS = -MT $@ -MMD -MP -MF $(DEPENDS_DIR)/$*.Td

