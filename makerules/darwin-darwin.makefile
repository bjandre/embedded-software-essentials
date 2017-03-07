#
# configuration options for native macOS
#
CC = cc
CFLAGS = $(DEFINE_LOGGER_ALGORITHM)=$(LOGGER_POLLING)

CPPFLAGS = -E

AR = ar

LD = ld
LDFLAGS = -macosx_version_min $(VERSION_MIN) -lSystem -map $*.map

SIZE = size
SIZEFLAGS = -m -l -x

MACOSX_VERSION := $(shell uname -r)
VERSION_MIN=10.11
ifeq ("$(MACOSX_VERSION)", "16.4.0")
  VERSION_MIN=10.12
endif

OBJDUMP = otool
OBJDUMP_FLAGS = -Vvt


