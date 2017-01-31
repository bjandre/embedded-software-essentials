# -*- mode: Makefile -*-
include ../../globals.makefile

EXE = main.out

SRCS = \
	main.c

OBJS = $(SRCS:%.c=%.o)

#
# Generic rule to generate objects
#
%.o : %.c 
	$(CC) $(CFLAGS) -c -o $@ $<

$(EXE) : $(OBJS)
	$(CC) $(CFLAGS) -o $@ $<

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



