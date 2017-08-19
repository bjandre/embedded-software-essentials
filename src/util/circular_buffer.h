
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef ESE_UTIL_CIRCULAR_BUFFER_H_
#define ESE_UTIL_CIRCULAR_BUFFER_H_

#include <stdbool.h>
#include <stddef.h>

#include "compiler_compat.h"

/**
   \file circular_buffer.h

   FIFO - circular buffer implementation.

   It is assumed that the primary use of the circular buffer is to move data
   into and out of critical regions of the code, e.g. between the main theread
   and interrupts. As a consequence, all circular buffer instances are required
   to be volatile.

   FIXME(bja, 2017-03) The project 2 API doesn't consider the use case of adding
   N items. If the user needs to add N items, but the available space is less
   than N, the user won't find out about it until after they have already added
   items to the buffer. If it doesn't make sense to add a partial block, then
   there is no way to remove the partial data (FIFO). So the user is stuck
   polling until it can add all the data. But they won't know how much data was
   added....

*/

typedef struct circular_buffer_t {
    void *buffer;/* Pointer to original memory location for the buffer */
    void *head;/* Pointer to Head */
    void *tail;/* Pointer to Tail */
    size_t bytes_used;/* Current count of items stored in the buffer */
    size_t num_items;/* total number of items that can be stored */
    size_t bytes_per_item;/* size of each item in bytes */
    size_t num_bytes_allocated;/* Allocated size of the buffer */
    void *buffer_end;/* End of the allocated buffer. */
} circular_buffer_t;


/**
   Status code returned by all circular buffer operations
 */
typedef enum circular_buffer_status_t {
    CIRCULAR_BUFFER_SUCCESS,
    CIRCULAR_BUFFER_IS_FULL,
    CIRCULAR_BUFFER_IS_EMPTY,
    CIRCULAR_BUFFER_NULL_POINTER,
    CIRCULAR_BUFFER_NO_NUM_ITEMS,
    CIRCULAR_BUFFER_NO_BYTES_PER_ITEMS,
    CIRCULAR_BUFFER_BUFFER_ALLOCATION_FAILURE,
    CIRCULAR_BUFFER_COPY_ERROR
} circular_buffer_status_t;

/**
   Adds an item to the circular_buffer.

   \param[in,out] *cb - pointer to circular buffer
   \param[in] item - to be added

   \return circular_buffer_status_t corresponding to action or buffer status
 */
circular_buffer_status_t circular_buffer_add_item(circular_buffer_t volatile
        *cb,
        void *item);

/**
   Removes an item from the buffer

   \param[in,out] *cb - pointer to circular buffer
   \param[out] *item - location to store removed item. Must be at least bytes_per_item in size.

   \return circular_buffer_status_t corresponding to action or buffer status
 */
circular_buffer_status_t circular_buffer_remove_item(circular_buffer_t volatile
        *cb,
        void *item);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
/**
   Report whether the buffer is full.

   \param[in,out] *cb - pointer to circular buffer
   \param[out] *is_full - boolean indicating if the buffer is full.

   \return circular_buffer_status_t corresponding to action or buffer status
            Note:
            this returns CIRCULAR_BUFFER_SUCCESS or circular_buffer__Null. CIRCULAR_BUFFER_IS_FULL only returned when
            trying to add an item to a full buffer.
 */
__attribute__( ( always_inline ) ) __STATIC_INLINE circular_buffer_status_t
circular_buffer_is_full(circular_buffer_t volatile *cb,
                        bool *is_full)
{
    circular_buffer_status_t status = CIRCULAR_BUFFER_SUCCESS;
    if (NULL == cb || NULL == is_full) {
        status = CIRCULAR_BUFFER_NULL_POINTER;
    } else {
        *is_full = false;
        if (cb->bytes_used > cb->num_bytes_allocated - cb->bytes_per_item) {
            *is_full = true;
        }
    }
    return status;
}


/**
   Report whether the buffer is empty.

   \param[in,out] *cb - pointer to circular buffer
   \param[out] *is_empty - boolean indicating if the buffer is empty.

   \return circular_buffer_status_t corresponding to action or buffer status
            Note:
            this returns CIRCULAR_BUFFER_SUCCESS or circular_buffer__Null. circular_buffer__Empyt only returned when
            trying to remove an item from an empty buffer.
 */
__attribute__( ( always_inline ) ) __STATIC_INLINE circular_buffer_status_t
circular_buffer_is_empty(circular_buffer_t volatile *cb,
                         bool *is_empty)
{
    circular_buffer_status_t status = CIRCULAR_BUFFER_SUCCESS;
    if (NULL == cb || NULL == is_empty) {
        status = CIRCULAR_BUFFER_NULL_POINTER;
    } else {
        *is_empty = false;
        if (cb->bytes_used < cb->bytes_per_item) {
            *is_empty = true;
        }
    }
    return status;
}

#pragma GCC diagnostic pop


/**
   Allows you to look at the nth item

   \param[in,out] *cb - pointer to circular buffer
   \param[in] index - index from the tail. Must be 0 <= index < num_items
   \param[out] *item - space to return the item, Must be at least bytes_per_item in size.

   \return circular_buffer_status_t corresponding to action or buffer status
 */
circular_buffer_status_t circular_buffer_peak_item(circular_buffer_t volatile
        *cb,
        const size_t index, void *item);

/**
   Allocates a new circular buffer structure and the buffer portion in memory
   (on the heap) given a number of bytes

   \param[in,out] *cb - pointer to circular buffer
   \param[in] num_items - number of items to store in the buffer.
   \param[in] bytes_per_item - the number of bytes per item stored in the buffer.

   \return circular_buffer_status_t corresponding to action or buffer status
 */
circular_buffer_status_t circular_buffer_new(volatile circular_buffer_t
        *volatile *cb,
        const size_t num_items,
        const size_t bytes_per_item);

/**
   Frees the circular buffer from dynamic memory

   \param[in,out] *cb - pointer to circular buffer

   \return circular_buffer_status_t corresponding to action or buffer status
 */
circular_buffer_status_t circular_buffer_destroy(volatile circular_buffer_t
        *volatile
        *cb);

#endif/* ESE_UTIL_CIRCULAR_BUFFER_H_ */
