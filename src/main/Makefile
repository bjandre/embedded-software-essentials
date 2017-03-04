# -*- mode: Makefile -*-
WORKSPACE_PATH := $(shell expr $(PWD) : '\(.*\)/src.*' \| $(PWD))
MAKERULES = $(WORKSPACE_PATH)/makerules

include $(MAKERULES)/config.makefile

EXE = $(EXE_BASE)$(PROJECT).$(EXE_EXTENSION)

BIN = $(EXE:%.$(EXE_EXTENSION)=%.$(BIN_EXTENSION))

SRCS = \
	main.c

OBJS = $(SRCS:%.c=%.o)

LIBS = \
	-L../util -lutil \
	-L../arch -larch

DEPEND_LIBS = \
	../arch/libarch.a \
	../project-1/libproject1.a \
	../util/libutil.a



INCLUDES = \
	-I../arch \
	-I../util \
	-I../project-1

TEST_EXE = $(EXE)

include $(MAKERULES)/generic.makefile

.PHONY : bin
bin : $(BIN)

$(BIN) : $(EXE)
ifdef BARE_METAL
	$(OBJCOPY) $(OBJCOPY_FLAGS) $< $@
#	$(CP) $@ $(OPENSDA_FLASH_MSD)/$@
endif
