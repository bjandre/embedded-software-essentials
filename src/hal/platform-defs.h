
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef ESE_HAL_PLATFORM_DEFS_H_
#define ESE_HAL_PLATFORM_DEFS_H_

/**
   \file platform-defs.h

   Contains global preprocssor definitions for controlling conditional
   compilation of platform specific code (hardware uart vs software printf,
   interrupt vs polling driven logging.
 */

#define PLATFORM_HOST (0) //!< macro for conditional compilation of host specific code
#define PLATFORM_BBB (1) //!< macro for conditional compilation of Beagle Bone Black specific code
#define PLATFORM_FRDM (2) //!< macro for conditional compilation of FRDM-KL25Z specific code

#define LOGGER_POLLING (0) //!< macro for conditional compilation of logging via polling
#define LOGGER_INTERRUPTS (1) //!< macro for conditional compilation of logging via interrupts

#endif /* ESE_HAL_PLATFORM_DEFS_H_ */
