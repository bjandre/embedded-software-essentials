#
# control build of embedded linux device tree overlays
#
# NOTE(bja, 2017-05) requires the dts 1.4.4 package from:
#
#  https://git.kernel.org/pub/scm/utils/dtc/dtc.git
#
# to be in the path.
#

DTC = dtc

DTC_FLAGS = --boot-cpu 0 --in-format dts --out-format dtb

SRCS = \
	bja-spi0-00a0.dts

OBJS = $(SRCS:%.dts=%.dtbo)

%.dtbo : %.dts
	$(DTC) $(DTC_FLAGS) -o $@ $<


all : $(OBJS)
