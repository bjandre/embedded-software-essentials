
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "data.h"
#include "memory.h"

DataStatus convert_endian32(uint32_t *data, uint32_t length);
bool is_whitespace(int8_t byte);

int8_t *my_itoa(int8_t *string, int32_t data, int32_t base)
{
    if (base > 16) {
        // don't support anything greater than hex
        return string;
    }
    if (NULL == string) {
        return string;
    }

    // offset for ascii characters
    int8_t ascii_offset = '0';
    // offset for ascii  hex characters > 9
    int8_t ascii_offset_hex = 'a' - 0x0a;

    uint32_t length = 0;
    int8_t *position = string;

    // insert the null terminator so it will be at the end when reversed
    *position = '\0';
    position++;
    length++;

    // record that the number was negative, then negate it to be positive.
    bool negative = false;
    if (data < 0) {
        negative = true;
        data = -data;
    }

    do {
        int8_t remainder = data % base;
        if (remainder > 9) {
            *position = remainder + ascii_offset_hex;
        } else {
            *position = remainder + ascii_offset;
        }
        position++;
        length++;
        data /= base;
    } while (data > 0);

    if (negative) {
        *position = '-';
        position++;
        length++;
    }
#   ifdef DEBUG_VERBOSE
    printf("itoa length = %" PRIu32 "   string = '%s'\n", length, string);
    for (uint32_t i = 0; i < 32; i++) {
        printf("<%c>", string[i]);
    }
    printf("\n");
#   endif

    my_reverse((uint8_t *)string, length);

#   ifdef DEBUG_VERBOSE
    printf("itoa string = '%s'\n", string);
    for (uint32_t i = 0; i < 32; i++) {
        printf("<%c>", string[i]);
    }
    printf("\n");
#   endif

    return string;
}

bool is_whitespace(int8_t byte)
{
    // check to see if a byte is an ascii whitespace character.
    bool is_space = false;
    switch (byte) {
    case ' ':
    case '\t':
    case '\n':
    case '\r':
        is_space = true;
        break;
    default:
        break;
    }
    return is_space;
}

int32_t my_atoi(int8_t *string)
{
    // assumes that digits are stored in the string such that the most
    // siginificant digit is at the lowest index in the string, e.g.
    // 1002 = str = {' ', ' ', '1', '0', '0', '2', '\0'}
    // only handles decimal strings.
    if (NULL == string) {
        return DataStatus_Success;
    }

    int8_t *position = string;

    // strip leading whitespace
    while (is_whitespace(*position)) {
        position++;
    }

    // check the sign
    uint32_t sign = 1;
    if (*position == '-') {
        sign = -1;
        position++;
    } else if (*position == '+') {
        position++;
    }

    // add the digits. every time we add a digit, we need to shift the previous
    // value by a single decimal digit.
    int32_t value = 0;
    while (*position >= '0' && *position <= '9') {
        value = 10 * value + (*position - '0');
        position++;
    }
    return sign * value;
}

/*
 * Convert an array of integers from one endian representation to another by
 * reversing the bypte order. The nuber of bytes is determined bye the size of
 * the input type.
 *
 * input:
 *
 *   data : pointer to a list of integers to be converted.
 *
 *   length : the number of integers to be converted.
 *
 * returns:
 *
 *   DataStatus_FAILURE if the conversion fails for any reason, otherwise DataStatus_Success
 *
 */
DataStatus convert_endian32(uint32_t *data, uint32_t length)
{
    // big endian - most significant byte is at the lowest memory location.
    // little endian - most significant byte is at the lowest memory location.

    // conversion from big to little and little to big is the same, just swap
    // the order of bytes within each integer....
    if (NULL == data) {
        return DataStatus_Null_Pointer;
    }
    size_t num_bytes = sizeof(*data);
    for (uint32_t i = 0; i < length; i++) {
        // each uint32 object is a chunk of num_bytes that get reversed.
        uint8_t *object = (uint8_t *)(data + i);
        MemStatus error = my_reverse(object, num_bytes);
        if (MemStatus_Success != error) {
            return DataStatus_Unknown_Error;
        }
    }
    return DataStatus_Success;
}

DataStatus big_to_little32(uint32_t *data, uint32_t length)
{
    return convert_endian32(data, length);
}

DataStatus little_to_big32(uint32_t *data, uint32_t length)
{
    return convert_endian32(data, length);
}

void print_memory(uint8_t *start, uint32_t length)
{
    // print a block of memory in hex.
    if (NULL == start) {
        return;
    }
    printf("%p", start);
    for (uint32_t i = 0; i < length; i++) {
        printf(" %.2x", *(start + i));
    }
}
