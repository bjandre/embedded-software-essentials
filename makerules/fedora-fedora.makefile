#
# configuration for native fedora builds
#

CC = cc
CFLAGS = 

CPPFLAGS = -E

AR = ar

SIZE = size
SIZEFLAGS = --format=sysv -x

OBJDUMP = objdump
OBJDUMP_FLAGS = --disassemble-all --line-numbers --source --full-contents --all-headers

LD = ld

