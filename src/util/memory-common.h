/**
   \file memory-common.h

   Common declarations shared between platform dependent and platform
   independent memory related utilities
 */

/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef ESE_UTIL_MEMORY_COMMON_H_
#define ESE_UTIL_MEMORY_COMMON_H_

#include <stdint.h>

/**
    Memory Utilities Return Status

    All memory utilities return the status of the operation.
 */
typedef enum MemoryStatus {
    MemStatus_Success,/*!< operation successful */
    MemStatus_Null_Pointer,/*!< operation failed with an unknow error */
    MemStatus_Transfer_Size_Error,/*!< invalid transfer size. must be 1, 2, or 4 bytes */
} MemStatus;

/**
 * Takes a pointer to a memory location and a length in
 * bytes and zeros out all of the memory.
 *
 * \param[in,out] destination pointer to a list of bytes
 * \param[in] length the number of bytes to be zeroed
 *
 * \return MemStatus flag indicating success or type of error that occurred.
 *
 */
MemStatus my_memzero(uint8_t *destination, uint32_t length);

/**
 * Takes a pointer to a memory location and a length in
 * bytes and reverses the order of the bytes
 *
 * \param[in,out] source pointer to a list of bytes
 * \param[in] length the number of bytes to be moved
 *
 * \return MemStatus flag indicating success or type of error that occurred.
 *
 */
MemStatus my_reverse(uint8_t *source, uint32_t length);


#endif /* ESE_UTIL_MEMORY_H_ */
