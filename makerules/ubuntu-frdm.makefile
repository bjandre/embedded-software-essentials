#
# configuration for ubuntu 16.04 - Freedom KL25z development board cross compiling
# with arm-none-eabi tool chain
#

include $(MAKERULES)/gcc-frdm.makefile

LDFLAGS += \
	-T "../arch/MKL25Z128xxx4_flash.ld" \
	--print-output-format \
	-L/usr/lib/gcc/arm-none-eabi/4.9.3/armv6-m \
	-L/usr/lib/gcc/arm-none-eabi/4.9.3/../../../arm-none-eabi/lib/armv6-m \
	-L/usr/lib/gcc/arm-none-eabi/4.9.3 \
	-L/usr/lib/gcc/arm-none-eabi/4.9.3/../../../arm-none-eabi/lib

LDLIBS = \
	/usr/lib/gcc/arm-none-eabi/4.9.3/armv6-m/crti.o \
	/usr/lib/gcc/arm-none-eabi/4.9.3/armv6-m/crtbegin.o \
	/usr/lib/gcc/arm-none-eabi/4.9.3/../../../arm-none-eabi/lib/armv6-m/crt0.o \
	--start-group -lgcc -lc -lrdimon --end-group \
	/usr/lib/gcc/arm-none-eabi/4.9.3/armv6-m/crtend.o \
	/usr/lib/gcc/arm-none-eabi/4.9.3/armv6-m/crtn.o


