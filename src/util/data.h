#ifndef __ESE_DATA_H__
#define __ESE_DATA_H__

#include <stdint.h>

/*
 * my_itoa(string, data, base)
 *
 * Convert a standard integer type into an ascii string. Handles signed data.
 *
 * Note: the return pointer value is the same as the input string.
 *
 */
int8_t *my_itoa(int8_t *string, int32_t data, int32_t base);

/*
 * my_atoi()
 *
 * Convert data from an ASCII represented string into an integer type. Handles
 * signed data.
 *
 * Returns the integer.
 *
 * Note the input parameter must be an null terminated string.
 * assumes that digits are stored in the string such that the most
 * siginificant digit is at the lowest index in the string, e.g.
 * 1002 = str = {'1', '0', '0', '2'}
 *
 */
int32_t my_atoi(int8_t *string);

/*
 * big_to_little32()
 *
 * Convert data types in memory from a big endian representation to little endian.
 *
 * Returns non-zero if the conversion fails for any reason.

*/
int8_t big_to_little32(uint32_t *data, uint32_t length);
/*
 * little_to_big32()
 *
 * Convert data types in memory from a little endian representation to big endian.
 *
 * Returns non-zero the conversion fails for any reason.
 *
 */
int8_t little_to_big32(uint32_t *data, uint32_t length);
/*
 * print_memory()
 *
 * Given a pointer to memory and a length in bytes, print the hex output of
 * bytes.
 *
 */
void print_memory(uint8_t *start, uint32_t length);

#endif /* __ESE_DATA_H__ */
