#ifndef ESE_UTIL_CIRCULAR_BUFFER_H_
#define ESE_UTIL_CIRCULAR_BUFFER_H_

typedef struct CircularBuffer_t CircularBuffer_t;

typedef enum CircularBufferStatus_t {
    CB_No_Error,
    CB_Full,
    CB_Empty,
    CB_Null_Pointer,
    CB_No_Length,
    CB_Buffer_Allocation_Failure
} CircularBufferStatus;

/**
   CircularBufferAddItem(circular_buffer, item)

   Adds an item to the circular_buffer.

   Param: *circ_buff - pointer to circular buffer
   Param: item - to be added

   Returns: CircularBufferStatus corresponding to action or buffer status
 */
CircularBufferStatus CircularBufferAddItem(CircularBuffer_t *circ_buff,
        void *item);

/**
   CircularBufferRemoveItem(circular_buffer, item)

   Removes an item from the buffer

   Param: *circ_buff - pointer to circular buffer
   Param: item - removed

   Returns: CircularBufferStatus corresponding to action or buffer status
 */
CircularBufferStatus CircularBufferRemoveItem(CircularBuffer_t *circ_buff);

/**
   CircularBufferFull(circular_buffer, item)

   Report whether the buffer is full.

   Param: *circ_buff - pointer to circular buffer

   Returns: CircularBufferStatus corresponding to action or buffer status
 */
CircularBufferStatus CircularBufferFull(CircularBuffer_t *circ_buff);


/**
   CircularBufferEmpty(circular_buffer, item)

   Report whether the buffer is empty.

   Param: *circ_buff - pointer to circular buffer

   Returns: CircularBufferStatus corresponding to action or buffer status
 */
CircularBufferStatus CircularBufferEmpty(CircularBuffer_t *circ_buff);


/**
   CircularBufferPeakItem(circular_buffer, index)

   Allows you to look at the nth item

   Param: *circ_buff - pointer to circular buffer
   Param: index. Must be less then count and greater or equal to zero.

   Returns: CircularBufferStatus corresponding to action or buffer status
 */
CircularBufferStatus CircularBufferPeakItem(CircularBuffer_t *circ_buff,
        const size_t index);

/**
   CircularBufferInit(circular_buffer, num_bytes)

   Allocates a circular buffer structure and the buffer portion in memory (on
   the heap) given a number of bytes

   Param: *circ_buff - pointer to circular buffer
   Param: num_bytes - the number of bytes to allocate for the buffer.

   Returns: CircularBufferStatus corresponding to action or buffer status
 */
CircularBufferStatus CircularBufferInit(CircularBuffer_t *circ_buff);

/**
   CircularBufferDestroy(circular_buffer)

   Frees the circular buffer from dynamic memory

   Param: *circ_buff - pointer to circular buffer

   Returns: CircularBufferStatus corresponding to action or buffer status
 */
CircularBufferStatus CircularBufferDestroy(CircularBuffer_t *circ_buff);

#endif // ESE_UTIL_CIRCULAR_BUFFER_H_
