# native compilation for os x, linux
NATIVE_CC = cc
NATIVE_CFLAGS = --std=c99 -Wall -g

# arm-linux-gnueabi cross compiler
ALG_X_CC = arm-linux-gnueabi-gcc
ALG_X_CFLAGS = --std=c99 -Wall -g

platform = unset
ifeq ($(platform), unset)
  CC = $(NATIVE_CC)
  CFLAGS = $(NATIVE_CFLAGS)
else
  ifeq ($(platform), bbb)
    CC = $(ALG_X_CC)
    CFLAGS = $(ALG_X_CFLAGS)
  endif
endif

HELLO_EXE = hello.out

HELLO_SRCS = hello.c

HELLO_OBJS = $(HELLO_SRCS:%.c=%.o)

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY : all
all : $(HELLO_EXE)

$(HELLO_EXE) : $(HELLO_OBJS)
	$(CC) $(CFLAGS) -o $@ $<


.PHONY : clean
clean : FORCE
	-rm -rf *.o *~ $(HELLO_EXE)

FORCE :


