#ifndef ESE_UTIL_MEMORY_H_
#define ESE_UTIL_MEMORY_H_

#include <stdint.h>

typedef enum MemoryStatus {
    MemStatus_SUCCESS,
    MemStatus_ERROR_NULL,
} MemStatus;

/*
 * my_memove(source, destination, length)
 *
 *
 * Takes one source pointer, one destination pointer and a length of bytes to
 * move from one location to the other.
 *
 * Works correctly for overlapped source and destination regions.
 *
 * The source and destination locations must have at least length bytes of
 * properly allocated memory.
 *
 * input:
 *
 *   source : pointer to a list of bytes to be moved.
 *
 *   destination : pointer to a destination location.
 *
 *   length : the number of bytes to be moved
 *
 * returns:
 *
 *   MemStatus flag indicating success or type of error tha occured.
 *
 */
MemStatus my_memmove(uint8_t *source, uint8_t *destination, uint32_t length);

/*
 * my_memset(source, length, value)
 *
 * Take a pointer to a memory location, a length in
 * bytes and 1 byte value. Set all bytes of the memory to the specified value.
 *
 * input:
 *
 *   source : pointer to a list of bytes
 *
 *   length : the number of bytes to be moved
 *
 *   value : 1 byte value to assign to all bytes.
 *
 * returns:
 *
 *   MemStatus flag indicating success or type of error tha occured.
 *
 */
MemStatus my_memset(uint8_t *source, uint32_t length, uint8_t value);

/*
 * my_memzero(source, length)
 *
 * Takes a pointer to a memory location and a length in
 * bytes and zeros out all of the memory.
 *
 * input:
 *
 *   source : pointer to a list of bytes
 *
 *   length : the number of bytes to be moved
 *
 * returns:
 *
 *   MemStatus flag indicating success or type of error tha occured.
 *
 */
MemStatus my_memzero(uint8_t *source, uint32_t length);

/*
 * my_reverse(source, length)
 *
 * Takes a pointer to a memory location and a length in
 * bytes and reverses the order of the bytes
 *
 * input:
 *
 *   source : pointer to a list of bytes
 *
 *   length : the number of bytes to be moved
 *
 * returns:
 *
 *   MemStatus flag indicating success or type of error tha occured.
 *
 */
MemStatus my_reverse(uint8_t *source, uint32_t length);


#endif /* ESE_UTIL_MEMORY_H_ */
