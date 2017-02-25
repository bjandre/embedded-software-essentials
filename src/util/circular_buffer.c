#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "memory.h"
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

void ClearCircularBuffer(CircularBuffer_t *cb);
CircularBufferStatus InitCircularBuffer(CircularBuffer_t *cb,
                                        const size_t num_items,
                                        const size_t bytes_per_item);
void *NewBufferPosition(CircularBuffer_t *cb, void *current_position);

void *NewBufferPosition(CircularBuffer_t *cb, void *current_position)
{
    void *new_position = current_position + cb->bytes_per_item;
    if (new_position > cb->buffer_end - cb->bytes_per_item) {
        new_position = cb->buffer;
    }
    return new_position;
}

CircularBufferStatus CircularBufferAddItem(CircularBuffer_t *cb,
        void *item)
{
    CircularBufferStatus status = CB_No_Error;
    if (NULL == item || NULL == cb) {
        status = CB_Null_Pointer;
    } else if (cb->bytes_used + cb->bytes_per_item > cb->num_bytes_allocated) {
        status = CB_Full;
    } else {
        MemStatus memstat = my_memmove(item, cb->head, cb->bytes_per_item);
        if (MemStatus_SUCCESS == memstat) {
            cb->head = NewBufferPosition(cb, cb->head);
            cb->bytes_used += cb->bytes_per_item;
        } else {
            status = CB_Copy_Error;
        }
    }
    return status;
}

CircularBufferStatus CircularBufferRemoveItem(CircularBuffer_t *cb, void *item)
{
    CircularBufferStatus status = CB_No_Error;
    if (NULL == item || NULL == cb) {
        status = CB_Null_Pointer;
    } else if (cb->bytes_used < cb->bytes_per_item) {
        status = CB_Empty;
    } else {
        MemStatus memstat = my_memmove(cb->tail, item, cb->bytes_per_item);
        if (MemStatus_SUCCESS == memstat) {
            cb->tail = NewBufferPosition(cb, cb->tail);
            cb->bytes_used -= cb->bytes_per_item;
        } else {
            status = CB_Copy_Error;
        }
    }
    return status;
}

CircularBufferStatus CircularBufferIsFull(CircularBuffer_t *cb, bool *is_full)
{
    CircularBufferStatus status = CB_No_Error;
    *is_full = false;
    if (cb->bytes_used > cb->num_bytes_allocated - cb->bytes_per_item) {
        *is_full = true;
    }
    return status;
}


CircularBufferStatus CircularBufferIsEmpty(CircularBuffer_t *cb, bool *is_empty)
{
    CircularBufferStatus status = CB_No_Error;
    *is_empty = false;
    if (cb->bytes_used < cb->bytes_per_item) {
        *is_empty = true;
    }
    return status;
}


CircularBufferStatus CircularBufferPeakItem(CircularBuffer_t *cb,
                                            const size_t index, void *item)
{
    CircularBufferStatus status = CB_No_Error;
    if (NULL == item || NULL == cb) {
        status = CB_Null_Pointer;
    } else {
        void *position = cb->tail + cb->bytes_per_item * index;
        if (position > cb->buffer_end - cb->bytes_per_item) {
            ptrdiff_t wrap_length = position - cb->buffer_end;
            position = cb->buffer + wrap_length;
        }
        MemStatus memstat = my_memmove(position, item, cb->bytes_per_item);
        if (MemStatus_SUCCESS == memstat) {
            // success!
        } else {
            status = CB_Copy_Error;
        }
    }
    return status;
}

CircularBufferStatus CircularBufferNew(CircularBuffer_t **cb,
                                       const size_t num_items,
                                       const size_t bytes_per_item)
{
    // NOTE: the result of malloc(0) is implementation defined, and is not
    // guarenteed to be a NULL pointer.
    CircularBufferStatus status = CB_No_Error;

    *cb = malloc(sizeof(CircularBuffer_t));

    if (NULL == *cb) {
        status = CB_Buffer_Allocation_Failure;
    } else {
        ClearCircularBuffer(*cb);
        status = InitCircularBuffer(*cb, num_items, bytes_per_item);
        if (CB_No_Error == status) {
        } else {
            // free any memory that might have been allocated. Don't save the
            // status because it's meaningless and we want to report the error
            // that was returned by Init.
            CircularBufferDestroy(cb);
        }
    }

    return status;
}

CircularBufferStatus InitCircularBuffer(CircularBuffer_t *cb,
                                       const size_t num_items,
                                       const size_t bytes_per_item)
{
    // NOTE: the result of malloc(0) is implementation defined, and is not
    // guarenteed to be a NULL pointer.

    CircularBufferStatus status = CB_No_Error;
    if (NULL == cb) {
        status = CB_Null_Pointer;
    }

    if (0 == num_items) {
        status = CB_No_Num_Items;
    }
    
    if (0 == bytes_per_item) {
        status = CB_No_Bytes_Per_Item;
    }

    if (CB_No_Error == status) {
        cb->num_items = num_items;
        cb->bytes_per_item = bytes_per_item;
        cb->num_bytes_allocated = cb->bytes_per_item *
            cb->num_items;
        
        cb->buffer = malloc(cb->num_bytes_allocated);
        if (NULL == cb->buffer) {
            status = CB_Buffer_Allocation_Failure;
        }
        cb->head = cb->buffer;
        cb->tail = cb->buffer;
        cb->buffer_end = cb->buffer + cb->num_bytes_allocated;
    }

    return status;
}

CircularBufferStatus CircularBufferDestroy(CircularBuffer_t **cb)
{
    if (NULL != *cb) {
        free((*cb)->buffer);
    }
    free(*cb);
    *cb = NULL;
    
    return CB_No_Error;
}

void ClearCircularBuffer(CircularBuffer_t *cb)
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
