
# Copyright 2017 Benjamin J. Andre.
# All Rights Reserved.
# 
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

#
# common configuration for FRDM-KL25Z cross compiling with
# arm-none-eabi
#
ARCH := armv6-m
CPU := cortex-m0plus
ARCH_FLAGS = -march=$(ARCH) -mcpu=$(CPU) -mthumb
DEFINE_CPU := CPU_MKL25Z128VLK4
BARE_METAL := 1
TOOLS_PREFIX := arm-none-eabi

CC_SPECS := --specs=nano.specs --specs=nosys.specs

CC = $(TOOLS_PREFIX)-gcc

CFLAGS = $(ARCH_FLAGS) -D$(DEFINE_CPU) -flto \
	$(DEFINE_LOGGER_ALGORITHM)=$(LOGGER_INTERRUPTS) \
	-DBARE_METAL=$(BARE_METAL)

AS = $(TOOLS_PREFIX)-gcc

ASMFLAGS = $(ARCH_FLAGS) -x assembler-with-cpp

AR = $(TOOLS_PREFIX)-gcc-ar

LD = $(TOOLS_PREFIX)-ld

LDFLAGS = \
	-T "../hal/frdm_kl25z/MKL25Z128xxx4_flash.ld" \
	--gc-sections -z muldefs \
	--print-output-format

CC_LDFLAGS = \
	-T "../hal/frdm_kl25z/MKL25Z128xxx4_flash.ld" \
	-Xlinker --gc-sections -Xlinker -z -Xlinker muldefs \
	-Xlinker --print-output-format \
	-Xlinker -Map=$(EXE).map \
	$(CC_SPECS)



# NOTE(bja, 2017-03) LDFLAGS is appened to for platform specific library paths.

SIZE = $(TOOLS_PREFIX)-size
SIZEFLAGS = --format=sysv -x

OBJDUMP = $(TOOLS_PREFIX)-objdump
OBJDUMP_FLAGS = --disassemble-all --line-numbers --source \
	--full-contents --all-headers --reloc --syms

ifeq ($(BIN_EXTENSION), srec)
  OBJFORMAT = $(BIN_EXTENSION)
else ifeq ($(BIN_EXTENSION), bin)
  OBJFORMAT = binary
else ifeq ($(BIN_EXTENSION), ihex)
  OBJFORMAT = $(BIN_EXTENSION)
else
  $(error Unsupported binary extension for objcopy : $(BIN_EXTENSION))
endif

OBJCOPY = $(TOOLS_PREFIX)-objcopy
OBJCOPY_FLAGS = -O $(OBJFORMAT)
