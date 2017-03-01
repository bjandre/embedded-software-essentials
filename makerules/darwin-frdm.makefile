#
# configuration for darwin - FRDM-KL25Z cross compiling with
# arm-none-eabi
#
ARCH := armv6-m
CPU := cortex-m0plus

BARE_METAL := 1

TOOLS_PREFIX := arm-none-eabi

DEFINE_CPU := CPU_MKL25Z128VLK4

CC = $(TOOLS_PREFIX)-gcc
CFLAGS = -march=$(ARCH) -mtune=$(CPU) -mthumb \
	-D$(DEFINE_CPU)

AR = $(TOOLS_PREFIX)-ar

LD = $(TOOLS_PREFIX)-ld
LDFLAGS = \
	--print-output-format \
	-L/usr/local/Caskroom/gcc-arm-embedded/5_4-2016q2,20160622/gcc-arm-none-eabi-5_4-2016q2/bin/../lib/gcc/arm-none-eabi/5.4.1/armv6-m \
	-L/usr/local/Caskroom/gcc-arm-embedded/5_4-2016q2,20160622/gcc-arm-none-eabi-5_4-2016q2/bin/../lib/gcc/arm-none-eabi/5.4.1/../../../../arm-none-eabi/lib/armv6-m \
	-L/usr/local/Caskroom/gcc-arm-embedded/5_4-2016q2,20160622/gcc-arm-none-eabi-5_4-2016q2/bin/../arm-none-eabi/lib/armv6-m \
	-L/usr/local/Caskroom/gcc-arm-embedded/5_4-2016q2,20160622/gcc-arm-none-eabi-5_4-2016q2/bin/../lib/gcc/arm-none-eabi/5.4.1 \
	-L/usr/local/Caskroom/gcc-arm-embedded/5_4-2016q2,20160622/gcc-arm-none-eabi-5_4-2016q2/bin/../lib/gcc \
	-L/usr/local/Caskroom/gcc-arm-embedded/5_4-2016q2,20160622/gcc-arm-none-eabi-5_4-2016q2/bin/../lib/gcc/arm-none-eabi/5.4.1/../../../../arm-none-eabi/lib \
	-L/usr/local/Caskroom/gcc-arm-embedded/5_4-2016q2,20160622/gcc-arm-none-eabi-5_4-2016q2/bin/../arm-none-eabi/lib --print-output-format \


LDLIBS = \
	/usr/local/Caskroom/gcc-arm-embedded/5_4-2016q2,20160622/gcc-arm-none-eabi-5_4-2016q2/bin/../lib/gcc/arm-none-eabi/5.4.1/armv6-m/crti.o \
	/usr/local/Caskroom/gcc-arm-embedded/5_4-2016q2,20160622/gcc-arm-none-eabi-5_4-2016q2/bin/../lib/gcc/arm-none-eabi/5.4.1/armv6-m/crtbegin.o \
	/usr/local/Caskroom/gcc-arm-embedded/5_4-2016q2,20160622/gcc-arm-none-eabi-5_4-2016q2/bin/../lib/gcc/arm-none-eabi/5.4.1/../../../../arm-none-eabi/lib/armv6-m/crt0.o \
	--start-group -lgcc -lc -lrdimon --end-group \
	/usr/local/Caskroom/gcc-arm-embedded/5_4-2016q2,20160622/gcc-arm-none-eabi-5_4-2016q2/bin/../lib/gcc/arm-none-eabi/5.4.1/armv6-m/crtend.o \
	/usr/local/Caskroom/gcc-arm-embedded/5_4-2016q2,20160622/gcc-arm-none-eabi-5_4-2016q2/bin/../lib/gcc/arm-none-eabi/5.4.1/armv6-m/crtn.o


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
