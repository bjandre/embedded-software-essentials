# -*- mode: Makefile -*-
WORKSPACE_PATH := $(shell expr $(PWD) : '\(.*\)/src.*' \| $(PWD))
MAKERULES = $(WORKSPACE_PATH)/makerules

include $(MAKERULES)/config.makefile

EXE = main.out

SRCS = \
	main.c

OBJS = $(SRCS:%.c=%.o)

LIBS = \
	-L../util -lutil

DEPEND_LIBS = \
	../project-1/libproject1.a \
	../util/libutil.a

INCLUDES = \
	-I../util \
	-I../project-1

CFLAGS += -DPROJECT=1

include $(MAKERULES)/generic.makefile

.PHONY : all
all : $(EXE)

.PHONY : compile-all
compile-all :

.PHONY : test
test : all
	./$(EXE)


# working
#file main.out
#main.out: ELF 64-bit LSB executable, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, for GNU/Linux 2.6.32, BuildID[sha1]=fc98840ebc15ddd0b8f895c7279fa7abeb7f7d51, not stripped
#ldd main.out
#	linux-vdso.so.1 =>  (0x00007fff4a1d6000)
#	libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007fb1e81cf000)
#	/lib64/ld-linux-x86-64.so.2 (0x000055dde08ac000)
#

# broken
#file main.out
#main.out: ELF 64-bit LSB executable, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib/ld64.so.1, for GNU/Linux 2.6.32, not stripped
#ldd main.out
#	linux-vdso.so.1 =>  (0x00007ffebe77b000)
#	libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007f12bd1b3000)
#	/lib/ld64.so.1 => /lib64/ld-linux-x86-64.so.2 (0x000055bea9c38000)
#
