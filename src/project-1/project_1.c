#include <assert.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "compatibility.h"
#include "data.h"
#include "memory.h"
#include "project_1.h"

void initialize_set_1(uint8_t *data, size_t size);
void initialize_set_2(uint8_t *data, size_t size);
void initialize_set_3(uint8_t *data, size_t size);
void test_data1(uint8_t *data, uint32_t size);
void test_data2(uint8_t *data, uint32_t size);
void test_memory(uint8_t *data, uint32_t size);

/*
 * void project_1_report()
 *
 * report function to demonstrate and test project 1
 *
 * This should call the functions below in order.
 */
void project_1_report()
{
    size_t const set_size = 32;

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
 * void initialize_set_1()
 *
 * set_1 : Create an array of 32 bytes (unsigned). Initialize this array to
 * increasing hex numbers like: 0x0123456789ABCDEF0123456789ABCDEF
 *
 */
void initialize_set_1(uint8_t *set_1, size_t size)
{
    // setting memory to repeating blocks of 0x0123456789abcdef is eight bytes.
    const uint32_t bytes_per_block = 8;
    assert(size % bytes_per_block == 0);
    int num_blocks = size / bytes_per_block;
    for (uint32_t block = 0; block < num_blocks ; block++) {
        uint32_t block_start = block * bytes_per_block;
        uint8_t *position = set_1 + block_start;
        // set the first byte in the block.
        *(position) = 0x01;
        for (uint32_t byte = 0; byte < bytes_per_block; byte++) {
            position++;
            *position = *(position - 1) + 0x22;
        }
    }

#   ifdef DEBUG_VERBOSE
    printf("set_1 :\n  ");
    print_memory(set_1, size);
    printf("\n");
#   endif
}

/*
 * void initialize_set_2()
 *
 * set_2 : Create another array of 32 bytes (unsigned). Initialize this array to
 * an increasing integer count of 1 ,2, 3, 4, 5, 6, 7, 8, 9, 10, 11, ...., 31.
 *
 */
void initialize_set_2(uint8_t *set_2, size_t size)
{
    for (uint32_t byte = 0; byte < size; byte++) {
        *(set_2 + byte) = byte;
    }

#   ifdef DEBUG_VERBOSE
    printf("set_2 :\n  ");
    print_memory(set_2, size);
    printf("\n");
#   endif
}

/*
 * void initialize_set_3()
 *
 * set_3 : Create another array of 32 bytes (unsigned). Initialize this array to
 * an increasing integer count of “abcdefghijklmnopqrstuvwxyzABCDEF”
 *
 */
void initialize_set_3(uint8_t *set_3, size_t size)
{
    for (uint32_t byte = 0; byte < size; byte++) {
        *(set_3 + byte) = 'a' + byte;
    }

#   ifdef DEBUG_VERBOSE
    printf("set_3 :\n  ");
    print_memory(set_3, size);
    printf("\n");
#   endif
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
        printf("%" UI32 " == '%s'\n  ", result, string);
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
 * 3. my_memset(); Set 0xEE on the 17th to the 20th bytes ([16]-[19])
 *
 * 4. my_memmove(); Move the 6 bytes starting at the 26th byte ([25]), to the
 *    20th position ([19])
 *
 * 5. memzero();  Bytes 12 - 16 ([11] - [15])
 *
 * 6. my_memmove(); Move the 8 bytes starting at the 1st byte ([0]), to the 9th
 *     position ([8])
 *
 * 7. print_memory(); all 32 bytes
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
    my_memset(position, length, value);
    print_memory(data, size);
    printf("\n  ");

    uint8_t *source = data + 25;
    uint8_t *destination = data + 19;
    length = 6;
    my_memmove(source, destination, length);
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
    my_memmove(source, destination, length);

    print_memory(data, size);
    printf("\n  ");
}
