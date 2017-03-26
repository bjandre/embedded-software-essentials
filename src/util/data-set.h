
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

/**
 * \file data-set.h
 *
 * Generic routines to initialize memory with known test patterns.
 *
 */

#ifndef ESE_UTIL_DATA_SET_H_
#define ESE_UTIL_DATA_SET_H_

#include <stdint.h>

/*
 * Initialize an array of bytes (unsigned) using increasing hex numbers like:
 * 0x0123456789ABCDEF0123456789ABCDEF
 *
 *   \param data pointer to memory to be initialize
 *
 *   \param size  number bytes to initialize, must be a multiple of 8 for this
 *   algorithm.
 *
 */
void initialize_set_1(uint8_t *data, uint32_t size);


/*
 * Initialize an array of bytes using an increasing integer count of 1 ,2, 3, 4,
 * 5, 6, 7, 8, 9, 10, 11, ...., 31.
 *
 *   \param data pointer to memory to be initialized
 *
 *   \param size number bytes to initialize
 *
 */
void initialize_set_2(uint8_t *data, uint32_t size);


/*
 * Initialize an array of bytes using an increasing integer count of
 * “abcdefghijklmnopqrstuvwxyzABCDEF”
 *
 *   \param data pointer to memory to be initialized
 *
 *   \param number bytes to initialize
 *
 */
void initialize_set_3(uint8_t *data, uint32_t size);

#endif // ESE_UTIL_DATA_SET_H_
