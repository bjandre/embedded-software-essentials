
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

/**
 * \file dma-host.h
 *
 * Platform specific memory functions using DMA. Since the host does not have
 * DMA, this is just a wrapper around the standard library calls.
 *
 */

#ifndef ESE_HAL_DMA_HOST_H_
#define ESE_HAL_DMA_HOST_H_

#include <stdint.h>

#include "memory-common.h"

/**
   Initialize the DMA.
 */
void host_initialize_dma(void);

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
 * \param[in] bytes_per_item number of bytes per item. Valid values: 1, 2, or 4
 *
 * \return MemStatus flag indicating success or type of error tha occurred.
 *
 * Check for overlapped regions. Note that there are five possible overlap patterns:
 *
 *   * src < src + len < dest < dest + len - no overlap, start at beginning of src.
 *
 *   * dest < dest + len < src < src + len - no overlap, start at beginning of src
 *
 *   * src < dest < src + len < dest + len - overlap - start at end of src
 *
 *   * dest < src < dest + len < src + len - overlap - start at beginning of src
 *
 *   * src == dest - complete overlap, nothing to do.
 *
 * Not that of the five possible combinations, there are three operations.
 *
 *   * Start beginning of src
 *
 *   * start at end of src
 *
 *   * do nothing
 */
MemStatus host_memmove_dma(uint8_t *destination, uint8_t const *const source,
                           uint32_t const num_items, uint8_t const bytes_per_item);

/**
 * Take a pointer to a destination memory location, a number of items, a source
 * memory location, and the number of bytes per item. Set all bytes of the
 * memory to the specified value.
 *
 * \param[in,out] destination pointer to a list of items
 * \param[in] source pointer to a list of items
 * \param[in] num_items the number of items to be moved
 * \param[in] bytes_per_item number of bytes per item. Valid values: 1, 2, or 4
 *
 * \returns MemStatus flag indicating success or type of error tha occurred.
 *
 */
MemStatus host_memset_dma(uint8_t *destination, uint8_t const *const source,
                          uint32_t const num_items, uint8_t const bytes_per_item);


#endif /* ESE_HAL_DMA_HOST_H_ */
