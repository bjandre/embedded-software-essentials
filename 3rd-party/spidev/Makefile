# NOTE(bja, 2017-07) as of the 4.9 linux kernel, spidev_test.c is in the linux
# source tree at:
#
# https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux-stable.git/tree/tools/spi/spidev_test.c?h=v4.9.38
#

all : spidev_test.c
	cc -o spidev_test spidev_test.c
	
test : all
	./spidev_test --verbose --speed 1000000 --device /dev/spidev1.0

