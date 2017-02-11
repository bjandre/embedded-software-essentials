#ifndef __ESE_MEMORY_H__
#define __ESE_MEMORY_H__

#include <stdint.h>

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
 *   EXIT_FAILURE if the conversion fails for any reason, otherwise EXIT_SUCCESS
 *
 */
int8_t my_memmove(uint8_t *source, uint8_t *destination, uint32_t length);

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
 *   EXIT_FAILURE if the conversion fails for any reason, otherwise EXIT_SUCCESS
 *
 */
int8_t my_memset(uint8_t *source, uint32_t length, uint8_t value);

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
 *   EXIT_FAILURE if the conversion fails for any reason, otherwise EXIT_SUCCESS
 *
 */
int8_t my_memzero(uint8_t *source, uint32_t length);

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
 *   EXIT_FAILURE if the conversion fails for any reason, otherwise EXIT_SUCCESS
 *
 */
int8_t my_reverse(uint8_t *source, uint32_t length);


#endif /* __ESE_MEMORY_H__ */
