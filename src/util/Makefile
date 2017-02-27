# -*- mode: Makefile -*-
WORKSPACE_PATH := $(shell expr $(PWD) : '\(.*\)/src.*' \| $(PWD))
MAKERULES = $(WORKSPACE_PATH)/makerules

include $(MAKERULES)/config.makefile

# subdirectories that will be built.
#SUBDIRS = 

LIB = \
	libutil.a

SRCS = \
	memory.c \
	data.c \
	circular_buffer.c \
	bit-operations.c

OBJS = $(SRCS:%.c=%.o)

INCLUDES = -I../arch \
	 -I$(CMOCKA_INCLUDE_DIR)


TEST_SRCS = \
	test_memory.c \
	test_data.c \
	test_circular_buffer.c \
	test_bit-operations.c \
	test_utils.c

TEST_OBJS = $(TEST_SRCS:%.c=%.o)
TEST_EXE = test_utils.out
TEST_DEPEND_LIBS = $(CMOCKA_LIBRARY)

include $(MAKERULES)/generic.makefile

$(TEST_EXE) : $(TEST_OBJS) $(OBJS)
	$(CC) $(CFLAGS) $(CMOCKA_INCLUDES) -O0 -g -U NDEBUG -o $@ $^ $(TEST_DEPEND_LIBS)


