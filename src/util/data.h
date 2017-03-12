
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef ESE_UTIL_DATA_H_
#define ESE_UTIL_DATA_H_

#include <stdint.h>

typedef enum DataUtilsStatus {
    DataStatus_SUCCESS,
    DataStatus_ERROR_NULL, // null pointer error
    DataStatus_ERROR_UNKNOWN, // unknown error
} DataStatus;

/*
 * my_itoa(string, data, base)
 *
 * Convert an integer type into an ascii string. Handles signed data.
 *
 * inputs:
 *
 *    string: byte array to return the resulting string
 *
 *    data: input integer to convert
 *
 *    base: numeric base to use for the conversion, arbitrary bases <= 16
 *    (hexidecimal) are supported.
 *
 * returns:
 *
 *   return pointer address is the same as the input string.
 *
 */
int8_t *my_itoa(int8_t *string, int32_t data, int32_t base);

/*
 * my_atoi()
 *
 * Convert data from an ASCII represented string into an integer type. Handles
 * signed data.
 *
 * input:
 *
 *   string: null terminated string / byte array
 *
 * returns:
 *
 *   the string converted to a 32 bit signed integer.
 *
 * Note the input parameter must be an null terminated string.
 * assumes that digits are stored in the string such that the most
 * siginificant digit is at the lowest index in the string, e.g.
 * 1002 = str = {'1', '0', '0', '2', '\0'}
 *
 */
int32_t my_atoi(int8_t *string);

/*
 * big_to_little32()
 *
 * Convert data types in memory from a big endian representation to little endian.
 *
 * input:
 *
 *   data : pointer to a list of integers to be converted.
 *
 *   length : the number of integers to be converted.
 *
 * returns:
 *
 *   DataUtilsStatus indicating where the conversion succeeded or the type of error.
 *
 */
DataStatus big_to_little32(uint32_t *data, uint32_t length);
/*
 * little_to_big32()
 *
 * Convert data types in memory from a little endian representation to big endian.
 *
 * input:
 *
 *   data : pointer to a list of integers to be converted.
 *
 *   length : the number of integers to be converted.
 *
 * returns:
 *
 *   DataUtilsStatus indicating where the conversion succeeded or the type of error.
 *
 */
DataStatus little_to_big32(uint32_t *data, uint32_t length);
/*
 * print_memory()
 *
 * Print a block of memory as a list of bytes in hex format.
 *
 * input:
 *
 *   data : pointer to a list of bytes
 *
 *   length : the number of bytes to be printed
 *
 */
void print_memory(uint8_t *start, uint32_t length);

#endif /* ESE_UTIL_DATA_H_ */
