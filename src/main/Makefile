# -*- mode: Makefile -*-
WORKSPACE_PATH := $(shell expr $(PWD) : '\(.*\)/src.*' \| $(PWD))
MAKERULES = $(WORKSPACE_PATH)/makerules

include $(MAKERULES)/config.makefile

EXE = $(EXE_BASE)$(PROJECT).$(EXE_EXTENSION)

BIN = $(EXE:%.$(EXE_EXTENSION)=%.$(BIN_EXTENSION))

SRCS = \
	main.c \
	debug-uart-data.c

OBJS = $(SRCS:%.c=%.o)

INCLUDES = \
	-I../arch \
	-I../util \
	-I../project-1

DEPEND_LIBS += \
	../project-1/libproject1.a \
	../arch/libarch.a

LIBS += \
	-L../project-1 -lproject1 \
	-L../arch -larch

ifeq ($(PLATFORM), $(PLATFORM_ENUM_FRDM))
  INCLUDES += \
	-I../arch/frdm-kl25z
  DEPEND_STARTUP += \
	../arch/frdm-kl25z/libfrdm_startup.a
  DEPEND_LIBS += \
	../arch/frdm-kl25z/libesefrdm.a 
  LIBS += \
	-L../arch/frdm-kl25z -lesefrdm
else ifeq ($(PLATFORM), $(PLATFORM_ENUM_HOST))
  INCLUDES += \
	-I../arch/host
  DEPEND_LIBS += \
	../arch/host/libesehost.a
  LIBS += \
	-L../arch/host -lesehost
endif

LIBS += \
	-L../util -lutil

DEPEND_LIBS += \
	../util/libutil.a

TEST_EXE = $(EXE)

include $(MAKERULES)/generic.makefile

.PHONY : bin
bin : $(BIN)

$(BIN) : $(EXE)
ifdef BARE_METAL
	$(OBJCOPY) $(OBJCOPY_FLAGS) $< $@
#	$(CP) $@ $(OPENSDA_FLASH_MSD)/$@
endif
