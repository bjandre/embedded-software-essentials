#
# generic gcc commands for native builds on the host.
#

CC = cc
CFLAGS = $(DEFINE_LOGGER_ALGORITHM)=$(LOGGER_POLLING)

CPPFLAGS = -E

AR = ar

LD = ld

SIZE = size
SIZEFLAGS = --format=sysv -x

OBJDUMP = objdump
OBJDUMP_FLAGS = --disassemble-all --line-numbers --source --full-contents --all-headers

