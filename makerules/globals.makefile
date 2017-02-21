# define some convenience variables 


DEPENDS_DIR := _depends
POSTCOMPILE = mv -f $(DEPENDS_DIR)/$*.Td $(DEPENDS_DIR)/$*.d

BUILD_COMPLETE = @echo Build complete: $@ : $(shell date)
#BUILD_COMPLETE =@echo '$(HOST_SYSTEM)'-'$(TARGET_SYSTEM)' build complete: $@ : $(shell date)

BUILD_ARTIFACTS = \
  *.o \
  *.asm \
  *.a \
  *.out \
  *.map \
  *.i \
  *.dump \
  $(DEPENDS_DIR) *.d *.Td

EDITOR_FILES = \
  *~ \
  *.astyle.orig


THIRD_PARTY_DIR := 3rd-party

CMOCKA_INCLUDE_DIR = $(WORKSPACE_PATH)/$(THIRD_PARTY_DIR)/build-Debug/include
CMOCKA_LIBRARY = $(WORKSPACE_PATH)/$(THIRD_PARTY_DIR)/build-Debug/lib/libcmocka.a
