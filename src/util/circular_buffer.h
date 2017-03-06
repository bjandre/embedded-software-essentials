#ifndef ESE_UTIL_CIRCULAR_BUFFER_H_
#define ESE_UTIL_CIRCULAR_BUFFER_H_

#include <stdbool.h>

typedef struct CircularBuffer_t CircularBuffer_t;

typedef enum CircularBufferStatus_t {
    CB_No_Error,
    CB_Full,
    CB_Empty,
    CB_Null_Pointer,
    CB_No_Num_Items,
    CB_No_Bytes_Per_Item,
    CB_Buffer_Allocation_Failure,
    CB_Copy_Error
} CircularBufferStatus;

/**
   CircularBufferAddItem(circular_buffer, item)

   Adds an item to the circular_buffer.

   Param: *cb - pointer to circular buffer
   Param: item - to be added

   Returns: CircularBufferStatus corresponding to action or buffer status
 */
CircularBufferStatus CircularBufferAddItem(CircularBuffer_t *cb,
        void *item);

/**
   CircularBufferRemoveItem(circular_buffer, item)

   Removes an item from the buffer

   Param: *cb - pointer to circular buffer
   Param: *item - location to store removed item. Must be at least bytes_per_item in size.

   Returns: CircularBufferStatus corresponding to action or buffer status
 */
CircularBufferStatus CircularBufferRemoveItem(CircularBuffer_t *cb,
        void *item);

/**
   CircularBufferIsFull(circular_buffer, is_full)

   Report whether the buffer is full.

   Param: *cb - pointer to circular buffer
   Param: *is_full - boolean indicating if the buffer is full.

   Returns: CircularBufferStatus corresponding to action or buffer status
            Note:
            this returns CB_No_Error or CB_Null. CB_Full only returned when
            trying to add an item to a full buffer.
 */
CircularBufferStatus CircularBufferIsFull(CircularBuffer_t *cb, bool *is_full);


/**
   CircularBufferIsEmpty(circular_buffer, is_empty)

   Report whether the buffer is empty.

   Param: *cb - pointer to circular buffer
   Param: *is_empty - boolean indicating if the buffer is empty.

   Returns: CircularBufferStatus corresponding to action or buffer status
            Note:
            this returns CB_No_Error or CB_Null. CB_Empyt only returned when
            trying to remove an item from an empty buffer.
 */
CircularBufferStatus CircularBufferIsEmpty(CircularBuffer_t *cb,
        bool *is_empty);


/**
   CircularBufferPeakItem(circular_buffer, index)

   Allows you to look at the nth item

   Param: *cb - pointer to circular buffer
   Param: index - index from the tail. Must be 0 <= index < num_items
   Param: *item - space to return the item, Must be at least bytes_per_item in size.

   Returns: CircularBufferStatus corresponding to action or buffer status
 */
CircularBufferStatus CircularBufferPeakItem(CircularBuffer_t *cb,
        const size_t index, void *item);

/**
   CircularBufferNew(circular_buffer, num_items, bytes_per_item)

   Allocates a new circular buffer structure and the buffer portion in memory
   (on the heap) given a number of bytes

   Param: *cb - pointer to circular buffer
   Param: num_items - number of items to store in the buffer.
   Param: bytes_per_item - the number of bytes per item stored in the buffer.

   Returns: CircularBufferStatus corresponding to action or buffer status
 */
CircularBufferStatus CircularBufferNew(CircularBuffer_t **cb,
                                       const size_t num_items,
                                       const size_t bytes_per_item);

/**
   CircularBufferDestroy(circular_buffer)

   Frees the circular buffer from dynamic memory

   Param: *cb - pointer to circular buffer

   Returns: CircularBufferStatus corresponding to action or buffer status
 */
CircularBufferStatus CircularBufferDestroy(CircularBuffer_t **cb);

#endif // ESE_UTIL_CIRCULAR_BUFFER_H_
