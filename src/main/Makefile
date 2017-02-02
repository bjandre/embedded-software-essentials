# -*- mode: Makefile -*-
include ../../makerules/globals.makefile
include ../../makerules/platform.makefile

EXE = main.out

SRCS = \
	main.c

OBJS = $(SRCS:%.c=%.o)

LIBS = \
	-L../util -lutil

DEPEND_LIBS = \
	../util/libutil.a

#
# Generic rule to generate objects
#
%.o : %.c 
	$(CC) $(CFLAGS) -c -o $@ $<

$(EXE) : $(OBJS) $(DEPEND_LIBS)
	$(LD) -o $@ $< $(LDFLAGS)

.PHONY : all
all : $(EXE)

.PHONY : comile-all
compile-all :

.PHONY : test
test : $(EXE)
	./$(EXE)

.PHONY : clean
clean :
	-rm -rf $(EDITOR_FILES) $(BUILD_ARTIFACTS)



