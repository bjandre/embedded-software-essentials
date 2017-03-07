# define some convenience variables 

LOGGER_POLLING := 0
LOGGER_INTERRUPTS := 1
DEFINE_LOGGER_ALGORITHM := -DLOGGER_ALGORITHM

EXE_BASE := project

EXE_EXTENSION := out
BIN_EXTENSION := bin
DUMP_EXTENSION := dump

DEPENDS_DIR := _depends
POSTCOMPILE = mv -f $(DEPENDS_DIR)/$*.Td $(DEPENDS_DIR)/$*.d

BUILD_COMPLETE = @echo Build complete: $@ : $(shell date)
#BUILD_COMPLETE =@echo '$(HOST_SYSTEM)'-'$(TARGET_SYSTEM)' build complete: $@ : $(shell date)

BUILD_ARTIFACTS = \
  *.o \
  *.asm \
  *.a \
  *.$(EXE_EXTENSION) \
  *.map \
  *.i \
  *.$(DUMP_EXTENSION) \
  *.$(SREC_EXTENSION) \
  $(DEPENDS_DIR) *.d *.Td

EDITOR_FILES = \
  *~ \
  *.astyle.orig


THIRD_PARTY_DIR := 3rd-party

CMOCKA_INCLUDE_DIR = $(WORKSPACE_PATH)/$(THIRD_PARTY_DIR)/build-Debug/include
CMOCKA_LIBRARY = $(WORKSPACE_PATH)/$(THIRD_PARTY_DIR)/build-Debug/lib/libcmocka.a
