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

CFLAGS = $(ARCH_FLAGS) -D$(DEFINE_CPU) -flto

AS = $(TOOLS_PREFIX)-as

ASMFLAGS = $(ARCH_FLAGS)

AR = $(TOOLS_PREFIX)-gcc-ar

LD = $(TOOLS_PREFIX)-ld

LDFLAGS = \
	-T "../arch/MKL25Z128xxx4_flash.ld" \
	--gc-sections -z muldefs \
	--print-output-format \


# NOTE(bja, 2017-03) LDFLAGS is appened to for platform specific library paths.

SIZE = $(TOOLS_PREFIX)-size
SIZEFLAGS = --format=sysv -x

OBJDUMP = $(TOOLS_PREFIX)-objdump
OBJDUMP_FLAGS = --disassemble-all --line-numbers --source --full-contents --all-headers

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
