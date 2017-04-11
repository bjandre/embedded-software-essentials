/**
   \file memory-cpu.h

   Platform and architecture dependent memory related utilities that utilize
   the DMA for memory manipulation.
 */

/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef ESE_HAL_MEMORY_DMA_H_
#define ESE_HAL_MEMORY_DMA_H_

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
 * \param[out] destination pointer to a destination location.
 * \param[in] source pointer to a list of bytes to be moved.
 * \param[in] num_items the number of items to be moved
 * \param[in] bytes_per_item the number of bytes per item
 *
 * \return MemStatus flag indicating success or type of error tha occurred.
 *
 */
MemStatus memmove_dma(uint8_t *destination, const uint8_t *const source,
                      uint32_t const num_items, uint32_t const bytes_per_item);

/**
 * Take a pointer to a memory location, a length in
 * bytes and 1 byte value. Set all bytes of the memory to the specified value.
 *
 * \param[out] destination pointer to a list of bytes
 * \param[in] source pointer to the 1 byte value to assign to all bytes.
 * \param[in] num_items the number of items to be moved
 * \param[in] bytes_per_item the number of bytes per item
 *
 * \returns MemStatus flag indicating success or type of error tha occurred.
 *
 */
MemStatus memset_dma(uint8_t *destination, const uint8_t *const source,
                     uint32_t const num_items, uint32_t const bytes_per_item);

#endif /* ESE_HAL_MEMORY_DMA_H_ */
