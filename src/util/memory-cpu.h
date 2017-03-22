/**
   \file memory-cpu.h

   Platform and architecture independent memory related utilities that utilize
   the cpu for memory manipulation. Platform specific implementations that use
   DMA will be more efficient.
 */

/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef ESE_UTIL_MEMORY_H_
#define ESE_UTIL_MEMORY_H_

#include <stdint.h>

#include "memory-common.h"

/**
 * Takes one source pointer, one destination pointer and a length of bytes to
 * move from one location to the other.
 *
 * Works correctly for overlapped source and destination regions.
 *
 * The source and destination locations must have at least length bytes of
 * properly allocated memory.
 *
 * \param[in] source pointer to a list of bytes to be moved.
 * \param[out] destination pointer to a destination location.
 * \param[in] length the number of bytes to be moved
 *
 * \return MemStatus flag indicating success or type of error tha occured.
 *
 */
MemStatus memmove_cpu(uint8_t *source, uint8_t *destination, uint32_t length);

/**
 * Take a pointer to a memory location, a length in
 * bytes and 1 byte value. Set all bytes of the memory to the specified value.
 *
 * \param[in,out] source pointer to a list of bytes
 * \param[in] length the number of bytes to be moved
 * \param[in] value 1 byte value to assign to all bytes.
 *
 * \returns MemStatus flag indicating success or type of error tha occured.
 *
 */
MemStatus memset_cpu(uint8_t *source, uint32_t length, uint8_t value);

#endif /* ESE_UTIL_MEMORY_H_ */
