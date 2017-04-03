
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

#include "memory-common.h"
#include "memory-cpu.h"
#include "circular_buffer.h"

struct CircularBuffer_t {
    void *buffer; // Pointer to original memory location for the buffer
    void *head; // Pointer to Head
    void *tail; // Pointer to Tail
    size_t bytes_used; // Current count of items stored in the buffer
    size_t num_items; // total number of items that can be stored
    size_t bytes_per_item; // size of each item in bytes
    size_t num_bytes_allocated; // Allocated size of the buffer
    void *buffer_end; // End of the allocated buffer.
};

/**
   ClearCircularBuffer()

   Set all internall pointers and sizes to NULL or zero.

   Params: cb - pointer to a circular buffer to be cleared.
 */
void ClearCircularBuffer(CircularBuffer_t volatile *cb);

/**

   InitCircularBuffer()

   Initialize a circular buffer, allocating internal memory, setting up
   pointers, and internal sizes.

   Param: cb - pointer to circular buffer
          num_items - number of items to allocate memory for. all items
                      must be the same size.
          bytes_per_item - the number of bytes per item to be stored.

 */
CircularBufferStatus InitCircularBuffer(CircularBuffer_t volatile *cb,
                                        const size_t num_items,
                                        const size_t bytes_per_item);

/**

   NewBufferposition()

   Calculate and return the next buffer position accounting for wrap around.

   Params: cb - pointer to the circular buffer.

           current_position - the current position to be incremented.

 */
void *NextBufferPosition(CircularBuffer_t volatile *cb, void *current_position);


void *NextBufferPosition(CircularBuffer_t volatile *cb, void *current_position)
{
    void *new_position = current_position + cb->bytes_per_item;
    if (new_position > cb->buffer_end - cb->bytes_per_item) {
        new_position = cb->buffer;
    }
    return new_position;
}

CircularBufferStatus CircularBufferAddItem(CircularBuffer_t volatile *cb,
        void *item)
{
    CircularBufferStatus status = CircularBuffer_Success;
    if (NULL == item || NULL == cb) {
        status = CircularBuffer_Null_Pointer;
    } else if (cb->bytes_used + cb->bytes_per_item > cb->num_bytes_allocated) {
        status = CircularBuffer_Is_Full;
    } else {
        MemStatus memstat = memmove_cpu(cb->head, item, cb->bytes_per_item);
        if (MemStatus_Success == memstat) {
            cb->head = NextBufferPosition(cb, cb->head);
            cb->bytes_used += cb->bytes_per_item;
        } else {
            status = CircularBuffer_Copy_Error;
        }
    }
    return status;
}

CircularBufferStatus CircularBufferRemoveItem(CircularBuffer_t volatile *cb,
        void *item)
{
    CircularBufferStatus status = CircularBuffer_Success;
    if (NULL == item || NULL == cb) {
        status = CircularBuffer_Null_Pointer;
    } else if (cb->bytes_used < cb->bytes_per_item) {
        status = CircularBuffer_Is_Empty;
    } else {
        MemStatus memstat = memmove_cpu(item, cb->tail, cb->bytes_per_item);
        if (MemStatus_Success == memstat) {
            cb->tail = NextBufferPosition(cb, cb->tail);
            cb->bytes_used -= cb->bytes_per_item;
        } else {
            status = CircularBuffer_Copy_Error;
        }
    }
    return status;
}

CircularBufferStatus CircularBufferIsFull(CircularBuffer_t volatile *cb,
        bool *is_full)
{
    CircularBufferStatus status = CircularBuffer_Success;
    if (NULL == cb || NULL == is_full) {
        status = CircularBuffer_Null_Pointer;
    } else {
        *is_full = false;
        if (cb->bytes_used > cb->num_bytes_allocated - cb->bytes_per_item) {
            *is_full = true;
        }
    }
    return status;
}


CircularBufferStatus CircularBufferIsEmpty(CircularBuffer_t volatile *cb,
        bool *is_empty)
{
    CircularBufferStatus status = CircularBuffer_Success;
    if (NULL == cb || NULL == is_empty) {
        status = CircularBuffer_Null_Pointer;
    } else {
        *is_empty = false;
        if (cb->bytes_used < cb->bytes_per_item) {
            *is_empty = true;
        }
    }
    return status;
}


CircularBufferStatus CircularBufferPeakItem(CircularBuffer_t volatile *cb,
        const size_t index, void *item)
{
    CircularBufferStatus status = CircularBuffer_Success;
    if (NULL == item || NULL == cb) {
        status = CircularBuffer_Null_Pointer;
    } else {
        void *position = cb->tail + cb->bytes_per_item * index;
        if (position > cb->buffer_end - cb->bytes_per_item) {
            ptrdiff_t wrap_length = position - cb->buffer_end;
            position = cb->buffer + wrap_length;
        }
        MemStatus memstat = memmove_cpu(item, position, cb->bytes_per_item);
        if (MemStatus_Success == memstat) {
            // success!
        } else {
            status = CircularBuffer_Copy_Error;
        }
    }
    return status;
}

CircularBufferStatus CircularBufferNew(volatile CircularBuffer_t *volatile *cb,
                                       const size_t num_items,
                                       const size_t bytes_per_item)
{
    // NOTE: the result of malloc(0) is implementation defined, and is not
    // guarenteed to be a NULL pointer.
    CircularBufferStatus status = CircularBuffer_Success;

    *cb = malloc(sizeof(CircularBuffer_t));

    if (NULL == *cb) {
        status = CircularBuffer_Buffer_Allocation_Failure;
    } else {
        ClearCircularBuffer(*cb);
        status = InitCircularBuffer(*cb, num_items, bytes_per_item);
        if (CircularBuffer_Success == status) {
        } else {
            // free any memory that might have been allocated. Don't save the
            // status because it's meaningless and we want to report the error
            // that was returned by Init.
            CircularBufferDestroy(cb);
        }
    }

    return status;
}

CircularBufferStatus InitCircularBuffer(CircularBuffer_t volatile *cb,
                                        const size_t num_items,
                                        const size_t bytes_per_item)
{
    // NOTE: the result of malloc(0) is implementation defined, and is not
    // guarenteed to be a NULL pointer.

    CircularBufferStatus status = CircularBuffer_Success;
    if (NULL == cb) {
        status = CircularBuffer_Null_Pointer;
    }

    if (0 == num_items) {
        status = CircularBuffer_No_Num_Items;
    }

    if (0 == bytes_per_item) {
        status = CircularBuffer_No_Bytes_Per_Item;
    }

    if (CircularBuffer_Success == status) {
        cb->num_items = num_items;
        cb->bytes_per_item = bytes_per_item;
        cb->num_bytes_allocated = cb->bytes_per_item *
                                  cb->num_items;

        cb->buffer = malloc(cb->num_bytes_allocated);
        if (NULL == cb->buffer) {
            status = CircularBuffer_Buffer_Allocation_Failure;
        }
        cb->head = cb->buffer;
        cb->tail = cb->buffer;
        cb->buffer_end = cb->buffer + cb->num_bytes_allocated;
    }

    return status;
}

CircularBufferStatus CircularBufferDestroy(volatile CircularBuffer_t *volatile
        *cb)
{
    if (NULL != *cb) {
        free((*cb)->buffer);
    }
    free((void *)*cb);
    *cb = NULL;

    return CircularBuffer_Success;
}

void ClearCircularBuffer(CircularBuffer_t volatile *cb)
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
