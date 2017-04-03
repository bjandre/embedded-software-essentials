
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

#include "data.h"
#include "data-set.h"
#include "memory-common.h"
#include "memory-cpu.h"
#include "project_1.h"

void test_data1(uint8_t *data, uint32_t size);
void test_data2(uint8_t *data, uint32_t size);
void test_memory(uint8_t *data, uint32_t size);

// FIXME(bja, 2017-02-11) Moving the definition of set_size as a constant into
// project_1_report results in a runtime failure when cross compiling for the
// BBB with a debug build. Cross compiling a release build is fine. Both release
// and native builds on the BBB are fine. The runtime failure is that the
// constant changes value from 32 to 17 between the last C instruction of
// initialize_set_1 and the creation of set_2 in main....? Some sort of stack
// corruption? Can't see anything useful in the cross compiled executable when
// running it in gdb.
uint32_t const set_size = 32;

/*
 * void project_1_report()
 *
 * report function to demonstrate and test project 1
 *
 * This should call the functions below in order.
 */
void project_1_report()
{
    uint8_t set_1[set_size];
    initialize_set_1(set_1, set_size);

    uint8_t set_2[set_size];
    initialize_set_2(set_2, set_size);

    uint8_t set_3[set_size];
    initialize_set_3(set_3, set_size);

    test_data1(set_1, set_size);
    test_data2(set_2, set_size);
    test_memory(set_2, set_size);
}

/*
 * void test_data1();
 *
 * Show your data functions work by calling the following functions in this
 * order with the first array, set_1 :
 *
 * 1. print_memory()
 * 2. big_to_little32()
 * 3. print_memory()
 * 4. little_to_big32()
 * 5. print_memory()
 *
 * input:
 *
 *   set_1 : pointer to input bytes
 *
 *   size : number bytes to manipulate
 *
 */
void test_data1(uint8_t *data, uint32_t size)
{
    printf("\ntest_data_1 :\n  ");

    // interpret the data as a bunch of 32bit, 4byte unsigned integers
    assert(size % sizeof(uint32_t) == 0);
    uint32_t *uint32_ptr = (uint32_t *)data;

    print_memory(data, size);
    printf("\n  ");
    big_to_little32(uint32_ptr, size);
    print_memory(data, size);
    printf("\n  ");
    little_to_big32(uint32_ptr, size);
    print_memory(data, size);
    printf("\n  ");
}

/*
 * void test_data2();
 *
 * Show your data functions work by calling the following functions in this
 * order with the second array, set_2
 *
 * 1. my_itoa()
 * 2. print_memory()
 * 3. my_atoi()
 * 4. print_memory()
 *
 * input:
 *
 *   set_1 : pointer to input bytes
 *
 *   size : number bytes to manipulate
 *
 */
void test_data2(uint8_t *data, uint32_t size)
{
    printf("\ntest_data_2 :\n  ");
    int8_t string[size];
    const int32_t base = 10;

    // interpret the data as 32bit, 4 byte signed integers. Loop over the 4 byte
    // ints for round trip conversion to and from a string.
    assert(size % sizeof(uint32_t) == 0);
    uint32_t num_ints = size / sizeof(int32_t);
    int32_t *data_ptr = (int32_t *)data;
    for (uint32_t i = 0; i < num_ints; i++) {
        my_itoa(string, *(data_ptr + i), base);
        print_memory(data, size);
        printf("\n  ");
        int32_t result = my_atoi(string);
        *(data_ptr + i) = result;
        print_memory(data, size);
        printf("\n  ");
        printf("compare int to string: %" PRIu32 " == '%s'\n  ", result, string);
    }
}


/*
 * void test_memory();
 *
 * Perform the following operations on a pointer to the set_2 data block.
 *
 * 1. print_memory(); all 32 bytes
 *
 * 2. my_reverse(); On the first 12 bytes
 *
 * 3. memset_cpu(); Set 0xEE on the 17th to the 20th bytes ([16]-[19])
 *
 * 4. memmove_cpu(); Move the 6 bytes starting at the 26th byte ([25]), to the
 *    20th position ([19])
 *
 * 5. memzero();  Bytes 12 - 16 ([11] - [15])
 *
 * 6. memmove_cpu(); Move the 8 bytes starting at the 1st byte ([0]), to the 9th
 *     position ([8])
 *
 * 7. print_memory(); all 32 bytes
 *
 * input:
 *
 *   set_1 : pointer to input bytes
 *
 *   size : number bytes to manipulate
 *
 */
void test_memory(uint8_t *data, uint32_t size)
{
    printf("\ntest_memory :\n  ");
    print_memory(data, size);
    printf("\n  ");

    uint8_t *position = data;
    uint32_t length = 12;
    my_reverse(position, length);
    print_memory(data, size);
    printf("\n  ");

    position = data + 16;
    length = 4;
    uint8_t value = 0xEE;
    memset_cpu(position, &value, length);
    print_memory(data, size);
    printf("\n  ");

    uint8_t *source = data + 25;
    uint8_t *destination = data + 19;
    length = 6;
    memmove_cpu(destination, source, length);
    print_memory(data, size);
    printf("\n  ");

    position = data + 11;
    length = 5;
    my_memzero(position, length);
    print_memory(data, size);
    printf("\n  ");

    source = data;
    destination = data + 8;
    length = 8;
    memmove_cpu(destination, source, length);

    print_memory(data, size);
    printf("\n  ");
}
