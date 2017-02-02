#ifndef __ESE_MEMORY_H__
#define __ESE_MEMORY_H__

#include <stdint.h>

/*
 * my_memove(source, destination, length)
 *
 *
 * Takes one source pointer, one destination pointer and a length of
 *  bytes to move from one location to the other.
 *
 * Works correctly for overlapped regions between the two memory
 * blocks.
 *
 * return an non-zero error if move fail for any reason.
 */
int8_t my_memmove(uint8_t *source, uint8_t *destination, uint32_t length);

/*
 * my_memset(source, length, value)
 *
 * This should take a pointer to a memory location, a length in
 * bytes and set all locations of that memory to a given value.
 *
 * Return an error if the function fails for any reason.
 *
 */
int8_t my_memset(uint8_t *source, uint32_t length, uint8_t value);

/*
 * my_memzero(source, length)
 *
 * This should take a pointer to a memory location and a length in
 * bytes and zero out all of the memory.
 *
 * return an error if the set fails for any reason.
 *
 */
int8_t my_memzero(uint8_t *source, uint32_t length);

/*
 * my_reverse(source, length)
 *
 * This should take a pointer to a memory location and a length in
 * bytes and reverse the order of all of the bytes
 *
 * Should return an error if the reverse fails for any reason.
 *
 */
int8_t my_reverse(uint8_t *source, uint32_t length);


#endif /* __ESE_MEMORY_H__ */
