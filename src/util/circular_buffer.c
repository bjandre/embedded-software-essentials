#include <stddef.h>

#include "circular_buffer.h"

struct CircularBuffer_t {
    void *buffer; // Pointer to original memory location for the buffer
    void *head; // Pointer to Head
    void *tail; // Pointer to Tail
    size_t length; // Allocated size of the buffer
    size_t count; // Current count of items stored in the buffer
};

CircularBufferStatus CircularBufferAddItem(CircularBuffer_t *circ_buff,
        void *item)
{
    return CB_Null_Pointer;
}

CircularBufferStatus CircularBufferRemoveItem(CircularBuffer_t *circ_buff)
{
    return CB_Null_Pointer;
}

CircularBufferStatus CircularBufferFull(CircularBuffer_t *circ_buff)
{
    return CB_Null_Pointer;
}


CircularBufferStatus CircularBufferEmpty(CircularBuffer_t *circ_buff)
{
    return CB_Null_Pointer;
}


CircularBufferStatus CircularBufferPeakItem(CircularBuffer_t *circ_buff,
        const size_t index)
{
    return CB_Null_Pointer;
}

CircularBufferStatus CircularBufferInit(CircularBuffer_t *circ_buff)
{
    return CB_Null_Pointer;
}

CircularBufferStatus CircularBufferDestroy(CircularBuffer_t *circ_buff)
{
    return CB_Null_Pointer;
}

