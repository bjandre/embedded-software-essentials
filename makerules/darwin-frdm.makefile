#
# configuration for darwin - FRDM-KL25Z cross compiling with
# arm-none-eabi
#
include $(MAKERULES)/gcc-frdm.makefile

COMPILER_ROOT = \
	/usr/local/Caskroom/gcc-arm-embedded/5_4-2016q2,20160622/gcc-arm-none-eabi-5_4-2016q2

LDFLAGS += \
	-plugin $(COMPILER_ROOT)/lib/gcc/arm-none-eabi/5.4.1/liblto_plugin.so \
	-plugin-opt=$(COMPILER_ROOT)/lib/gcc/arm-none-eabi/5.4.1/lto-wrapper \
	-plugin-opt=-pass-through=-lgcc \
	-plugin-opt=-pass-through=-lg_nano \
	-plugin-opt=-pass-through=-lc_nano \
	-plugin-opt=-pass-through=-lgcc \
	-plugin-opt=-pass-through=-lc_nano \
	-plugin-opt=-pass-through=-lnosys \
	-plugin-opt=-pass-through=-lgcc \
	-plugin-opt=-pass-through=-lc_nano \
	-plugin-opt=-pass-through=-lnosys \
	--sysroot=$(COMPILER_ROOT)/arm-none-eabi \
	-X \
	-L$(COMPILER_ROOT)/lib/gcc/arm-none-eabi/5.4.1/armv6-m \
	-L$(COMPILER_ROOT)/arm-none-eabi/lib/armv6-m \
	-L$(COMPILER_ROOT)/lib/gcc/arm-none-eabi/5.4.1 \
	-L$(COMPILER_ROOT)/lib/gcc \
	-L$(COMPILER_ROOT)/arm-none-eabi/lib

LDLIBS_PRE = \
	$(COMPILER_ROOT)/lib/gcc/arm-none-eabi/5.4.1/armv6-m/crti.o \
	$(COMPILER_ROOT)/lib/gcc/arm-none-eabi/5.4.1/armv6-m/crtbegin.o \
	$(COMPILER_ROOT)/arm-none-eabi/lib/armv6-m/crt0.o

LDLIBS_POST = \
	--start-group -lgcc -lg_nano -lc_nano --end-group \
	--start-group -lgcc -lc_nano -lnosys --end-group \
	--start-group -lgcc -lc_nano -lnosys --end-group \
	$(COMPILER_ROOT)/lib/gcc/arm-none-eabi/5.4.1/armv6-m/crtend.o \
	$(COMPILER_ROOT)/lib/gcc/arm-none-eabi/5.4.1/armv6-m/crtn.o
