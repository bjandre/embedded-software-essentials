
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "memory_common.h"
#include "memory_cpu.h"
#include "circular_buffer.h"

/**
   Clearcircular_buffer_()

   Set all internall pointers and sizes to NULL or zero.

   Params: cb - pointer to a circular buffer to be cleared.
 */
void Clearcircular_buffer_(circular_buffer_t volatile *cb);

/**

   Initcircular_buffer_()

   Initialize a circular buffer, allocating internal memory, setting up
   pointers, and internal sizes.

   Param: cb - pointer to circular buffer
          num_items - number of items to allocate memory for. all items
                      must be the same size.
          bytes_per_item - the number of bytes per item to be stored.

 */
circular_buffer_status_t Initcircular_buffer_(circular_buffer_t volatile *cb,
        const size_t num_items,
        const size_t bytes_per_item);

/**

   NewBufferposition()

   Calculate and return the next buffer position accounting for wrap around.

   Params: cb - pointer to the circular buffer.

           current_position - the current position to be incremented.

 */
void *NextBufferPosition(circular_buffer_t volatile *cb,
                         void *current_position);


void *NextBufferPosition(circular_buffer_t volatile *cb, void *current_position)
{
    void *new_position = current_position + cb->bytes_per_item;
    if (new_position > cb->buffer_end - cb->bytes_per_item) {
        new_position = cb->buffer;
    }
    return new_position;
}

circular_buffer_status_t circular_buffer_add_item(circular_buffer_t volatile
        *cb,
        void *item)
{
    circular_buffer_status_t status = CIRCULAR_BUFFER_SUCCESS;
    if (NULL == item || NULL == cb) {
        status = CIRCULAR_BUFFER_NULL_POINTER;
    } else if (cb->bytes_used + cb->bytes_per_item > cb->num_bytes_allocated) {
        status = CIRCULAR_BUFFER_IS_FULL;
    } else {
        MemStatus memstat = memmove_cpu(cb->head, item, cb->bytes_per_item);
        if (MemStatus_Success == memstat) {
            cb->head = NextBufferPosition(cb, cb->head);
            cb->bytes_used += cb->bytes_per_item;
        } else {
            status = CIRCULAR_BUFFER_COPY_ERROR;
        }
    }
    return status;
}

circular_buffer_status_t circular_buffer_remove_item(circular_buffer_t volatile
        *cb,
        void *item)
{
    circular_buffer_status_t status = CIRCULAR_BUFFER_SUCCESS;
    if (NULL == item || NULL == cb) {
        status = CIRCULAR_BUFFER_NULL_POINTER;
    } else if (cb->bytes_used < cb->bytes_per_item) {
        status = CIRCULAR_BUFFER_IS_EMPTY;
    } else {
        MemStatus memstat = memmove_cpu(item, cb->tail, cb->bytes_per_item);
        if (MemStatus_Success == memstat) {
            cb->tail = NextBufferPosition(cb, cb->tail);
            cb->bytes_used -= cb->bytes_per_item;
        } else {
            status = CIRCULAR_BUFFER_COPY_ERROR;
        }
    }
    return status;
}


circular_buffer_status_t circular_buffer_peak_item(circular_buffer_t volatile
        *cb,
        const size_t index, void *item)
{
    circular_buffer_status_t status = CIRCULAR_BUFFER_SUCCESS;
    if (NULL == item || NULL == cb) {
        status = CIRCULAR_BUFFER_NULL_POINTER;
    } else {
        void *position = cb->tail + cb->bytes_per_item * index;
        if (position > cb->buffer_end - cb->bytes_per_item) {
            ptrdiff_t wrap_length = position - cb->buffer_end;
            position = cb->buffer + wrap_length;
        }
        MemStatus memstat = memmove_cpu(item, position, cb->bytes_per_item);
        if (MemStatus_Success == memstat) {
            /* success! */
        } else {
            status = CIRCULAR_BUFFER_COPY_ERROR;
        }
    }
    return status;
}

circular_buffer_status_t circular_buffer_new(volatile circular_buffer_t
        *volatile *cb,
        const size_t num_items,
        const size_t bytes_per_item)
{
    /* NOTE: the result of malloc(0) is implementation defined, and is not */
    /* guarenteed to be a NULL pointer. */
    circular_buffer_status_t status = CIRCULAR_BUFFER_SUCCESS;

    *cb = malloc(sizeof(circular_buffer_t));

    if (NULL == *cb) {
        status = CIRCULAR_BUFFER_BUFFER_ALLOCATION_FAILURE;
    } else {
        Clearcircular_buffer_(*cb);
        status = Initcircular_buffer_(*cb, num_items, bytes_per_item);
        if (CIRCULAR_BUFFER_SUCCESS == status) {
        } else {
            /* free any memory that might have been allocated. Don't save the */
            /* status because it's meaningless and we want to report the error */
            /* that was returned by Init. */
            circular_buffer_destroy(cb);
        }
    }

    return status;
}

circular_buffer_status_t Initcircular_buffer_(circular_buffer_t volatile *cb,
        const size_t num_items,
        const size_t bytes_per_item)
{
    /* NOTE: the result of malloc(0) is implementation defined, and is not */
    /* guarenteed to be a NULL pointer. */

    circular_buffer_status_t status = CIRCULAR_BUFFER_SUCCESS;
    if (NULL == cb) {
        status = CIRCULAR_BUFFER_NULL_POINTER;
    }

    if (0 == num_items) {
        status = CIRCULAR_BUFFER_NO_NUM_ITEMS;
    }

    if (0 == bytes_per_item) {
        status = CIRCULAR_BUFFER_NO_BYTES_PER_ITEMS;
    }

    if (CIRCULAR_BUFFER_SUCCESS == status) {
        cb->num_items = num_items;
        cb->bytes_per_item = bytes_per_item;
        cb->num_bytes_allocated = cb->bytes_per_item *
                                  cb->num_items;

        cb->buffer = malloc(cb->num_bytes_allocated);
        if (NULL == cb->buffer) {
            status = CIRCULAR_BUFFER_BUFFER_ALLOCATION_FAILURE;
        }
        cb->head = cb->buffer;
        cb->tail = cb->buffer;
        cb->buffer_end = cb->buffer + cb->num_bytes_allocated;
    }

    return status;
}

circular_buffer_status_t circular_buffer_destroy(volatile circular_buffer_t
        *volatile
        *cb)
{
    if (NULL != *cb) {
        free((*cb)->buffer);
    }
    free((void *)*cb);
    *cb = NULL;

    return CIRCULAR_BUFFER_SUCCESS;
}

void Clearcircular_buffer_(circular_buffer_t volatile *cb)
{
    cb->buffer = NULL;
    cb->head = NULL;
    cb->tail = NULL;
    cb->buffer_end = NULL;
    cb->bytes_used = 0;
    cb->num_items = 0;
    cb->bytes_per_item = 0;
    cb->num_bytes_allocated = 0;
}
