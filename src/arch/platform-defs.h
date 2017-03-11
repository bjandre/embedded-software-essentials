#ifndef ESE_ARCH_PLATFORM_DEFS_H_
#define ESE_ARCH_PLATFORM_DEFS_H_

/**
   Contains global preprocssor definitions for controlling conditional
   compilation of platform specific code (hardware uart vs software printf,
   interrupt vs polling driven logging.
 */

#define PLATFORM_HOST (0)
#define PLATFORM_BBB (1)
#define PLATFORM_FRDM (2)

#define LOGGER_POLLING (0)
#define LOGGER_INTERRUPTS (1)

#endif /* ESE_ARCH_PLATFORM_DEFS_H_ */
