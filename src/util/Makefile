# -*- mode: Makefile -*-
include ../../makerules/globals.makefile
include ../../makerules/platform.makefile

# subdirectories that will be built.
#SUBDIRS = 

LIB = \
	libutil.a

SRCS = \
	memory.c

OBJS = $(SRCS:%.c=%.o)

#
# macro for executing TARGET in all SUBDIRS
#
ifdef SUBDIRS
.PHONY : $(SUBDIRS)
$(SUBDIRS) : 
	@if [ -d $@ ]; then \
		$(MAKE) --directory=$@ $(MAKECMDGOALS); \
	fi	
	@echo build complete: $@ : $(shell date)
endif	

#
# Generic rule to generate objects
#
%.o : %.c 
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIB) : $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

.PHONY : all
all : $(SUBDIRS) $(LIB)

.PHONY : comile-all
compile-all : $(SUBDIRS)

.PHONY : test
test : $(SUBDIRS) all

.PHONY : clean
clean : $(SUBDIRS)
	-rm -rf $(EDITOR_FILES) $(BUILD_ARTIFACTS)



