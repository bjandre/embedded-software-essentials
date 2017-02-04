# -*- mode: Makefile -*-


DEPENDS_DIR := _depends
POSTCOMPILE = mv -f $(DEPENDS_DIR)/$*.Td $(DEPENDS_DIR)/$*.d

BUILD_COMPLETE = @echo Build complete: $@ : $(shell date)

BUILD_ARTIFACTS = \
  *.o \
  *.asm \
  *.a \
  *.out \
  *.map \
  *.i \
  $(DEPENDS_DIR) *.d *.Td

EDITOR_FILES = \
  *~ \
  *.astyle.orig
