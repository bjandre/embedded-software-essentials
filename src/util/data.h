#ifndef __ESE_DATA_H__
#define __ESE_DATA_H__

#include <stdint.h>

/*
 * my_itoa(string, data, base)
 *
 * Convert a standard integer type into an ascii string. Handles signed data.
 *
 */
int8_t *my_itoa(int8_t *string, int32_t data, int32_t base);

/*
 * my_()
 *
 * i. AsciI-to-Integer needs to convert data back from an ASCII represented string into an integer type. Need to handle signed data.
 */
int32_t my_atoi(int8_t *string);

/*
 * my_()
 *
 *i. Needs to be able to convert data types in memory from a big endian representation to little endian.
 ii. Should return an error if the conversion fails for any reason.

*/
int8_t big_to_little32(uint32_t *data, uint32_t length);
/*
 * my_()
 *
 i. Needs to be able to convert data types in memory from a little endian representation to big endian.
 ii. Should return an error if the conversion fails for any reason.
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
