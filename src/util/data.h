/**
   \file data.h

   Platform and architecture independent data manipulation utilities
 */

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

/**
   Data Utilities Return Status

   All data utilities return the status of the operation.
 */
typedef enum DataUtilsStatus {
    DataStatus_Success,/*!< operation successful */
    DataStatus_Null_Pointer,/*!< received null pointer error */
    DataStatus_Unknown_Error,/*!< unknown error */
} DataStatus;

/**
 * Convert an integer type into an ascii string. Handles signed data.
 *
 * \param[out] string byte array to return the resulting string
 * \param[in] data input integer to convert
 * \param[in] base numeric base to use for the conversion, arbitrary bases <= 16
 *    (hexidecimal) are supported.
 *
 * \return pointer address is the same as the input string.
 *
 */
int8_t *my_itoa(int8_t *string, int32_t data, int32_t base);

/**
 * Convert data from an ASCII represented string into an integer type. Handles
 * signed data.
 *
 * Note the input parameter must be an null terminated string.
 * assumes that digits are stored in the string such that the most
 * siginificant digit is at the lowest index in the string, e.g.
 * 1002 = str = {'1', '0', '0', '2', '\0'}
 *
 * \param[in] string null terminated string / byte array
 *
 * \returns the string converted to a 32 bit signed integer.
 *
 */
int32_t my_atoi(int8_t *string);

/**
 * Convert data types in memory from a big endian representation to little endian.
 *
 * \param[in,out] data pointer to a list of integers to be converted.
 * \param[in] length the number of integers to be converted.
 *
 * \return DataUtilsStatus indicating where the conversion succeeded or the type of error.
 *
 */
DataStatus big_to_little32(uint32_t *data, uint32_t length);

/**
 * Convert data types in memory from a little endian representation to big endian.
 *
 * \param[in,out] data pointer to a list of integers to be converted.
 * \param[in] length the number of integers to be converted.
 *
 * \return DataUtilsStatus indicating where the conversion succeeded or the type of error.
 *
 */
DataStatus little_to_big32(uint32_t *data, uint32_t length);

/**
 * Print a block of memory as a list of bytes in hex format.
 *
 * \param[in] start pointer to a list of bytes
 * \param[in] length the number of bytes to be printed
 *
 */
void print_memory(uint8_t *start, uint32_t length);

#endif /* ESE_UTIL_DATA_H_ */
