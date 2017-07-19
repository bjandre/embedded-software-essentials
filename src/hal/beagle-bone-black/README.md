# Beagle Bone Black setup

## Device Tree

Setup the device tree to enable spi0, cs0 using pins:

SPI0 CS0 - port 9, pin 17
SPI0 D1 - port 9 pin 18 - MOSI
SPI0 D0 - port 9 pin 21 - MISO
SPI0 CLK - port 9 pin 22

Edit the /boot/uEnv.txt file to enable overlays, disable the universal cape,
disable the video, wireless and adc. Enable BB-SPIDEV0:

    sudo /opt/scripts/tools/version.sh

    git:/opt/scripts/:[ddd67f07548ebbc1884b2f52d16af900b8c7a159]
    eeprom:[A335BNLT00C02916BBBK1656]
    dogtag:[BeagleBoard.org Debian Image 2017-07-09]
    bootloader:[eMMC-(default)]:[/dev/mmcblk1]:[U-Boot 2017.07-rc3-00002-g181e31]
    kernel:[4.9.36-ti-r45]
    uboot_overlay_options:[enable_uboot_overlays=1]
    uboot_overlay_options:[uboot_overlay_addr0=/lib/firmware/BB-SPIDEV0-00A0.dtbo]
    uboot_overlay_options:[disable_uboot_overlay_video=1]
    uboot_overlay_options:[disable_uboot_overlay_audio=1]
    uboot_overlay_options:[disable_uboot_overlay_wireless=1]
    uboot_overlay_options:[disable_uboot_overlay_adc=1]
    uboot_overlay_options:[uboot_overlay_pru=/lib/firmware/AM335X-PRU-UIO-00A0.dtbo]



