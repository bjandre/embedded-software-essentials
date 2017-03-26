
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

/**
 * \file dma-frdm-kl25z.h
 *
 * Platform specific memory functions using DMA
 *
 */

#ifndef ESE_ARCH_DMA_FRDM_KL25Z_H_
#define ESE_ARCH_DMA_FRDM_KL25Z_H_

#include <stdint.h>

#include "memory-common.h"

/**
   Initialize the DMA.
 */
void frdm_kl25z_initialize_dma(void);

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
 * \return MemStatus flag indicating success or type of error tha occured.
 *
 */
MemStatus memmove_dma(uint8_t *destination, uint8_t *source, uint32_t num_items,
                      uint8_t bytes_per_item);

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
 * \returns MemStatus flag indicating success or type of error tha occured.
 *
 */
MemStatus memset_dma(uint8_t *destination, uint8_t *source, uint32_t num_items,
                     uint8_t bytes_per_item);


#endif /* ESE_ARCH_DMA_FRDM_KL25Z_H_ */