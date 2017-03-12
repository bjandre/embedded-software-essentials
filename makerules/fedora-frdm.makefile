
# Copyright 2017 Benjamin J. Andre.
# All Rights Reserved.
# 
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

#
# configuration for fedora - FRDM-KL25Z cross compiling with
# arm-none-eabi
#

# NOTE(bja, 2017-03) compilers are installed as part of kinetis design
# studio. Need to set the path to use this configuration:
# PATH=$PATH:/opt/Freescale/KDS_v3/toolchain/bin

include $(MAKERULES)/gcc-frdm.makefile

