#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "compatibility.h"
#include "data.h"
#include "memory.h"
#include "project_1.h"

size_t const set_size = 32;

void initialize_set_1(uint8_t *data, size_t size);
void initialize_set_2(uint8_t *data, size_t size);
void initialize_set_3(uint8_t *data, size_t size);
void test_data1(uint8_t *data, uint32_t size);
void test_data2(uint8_t *data, uint32_t size);
void test_memory(uint8_t *data, uint32_t size);

/*
 * void project_1_report()
 *
 * report function to test project 1
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
 * void initialize_set_1()
 *
 * set_1 : Create an array of 32 bytes (unsigned). Initialize this array to
 * increasing hex numbers like: 0x0123456789ABCDEF0123456789ABCDEF
 *
 */
void initialize_set_1(uint8_t *set_1, size_t size)
{
    for (uint32_t s = 0; s < set_size / 8; s++) {
        uint32_t offset = s * 8;
        uint8_t *position = set_1 + offset;
        *(position) = 0x01;
        for (uint32_t i = 1; i < 8; i++) {
            position++;
            *position = *(position - 1) + 0x22;
        }
    }

    if (0) {
        printf("set_1 :\n  ");
        print_memory(set_1, set_size);
        printf("\n");
    }
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
    for (uint32_t i = 0; i < set_size; i++) {
        *(set_2 + i) = i;
    }

    if (0) {
        printf("set_2 :\n  ");
        print_memory(set_2, set_size);
        printf("\n");
    }
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
    for (uint32_t i = 0; i < set_size; i++) {
        *(set_3 + i) = 'a' + i;
    }
    if (0) {
        printf("set_3 :\n  ");
        print_memory(set_3, set_size);
        printf("\n");
    }
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
    print_memory(data, size);
    printf("\n  ");
    big_to_little32((uint32_t *)data, size);
    print_memory(data, size);
    printf("\n  ");
    little_to_big32((uint32_t *)data, size);
    print_memory(data, size);
    printf("\n");
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
    int8_t string[set_size];
    int32_t base = 10;
    my_itoa(string, *(int32_t *)data, base);
    print_memory(data, size);
    printf("\n  ");
    int32_t result = my_atoi(string);
    print_memory(data, size);
    printf("\n  ");
    printf("%" UI32 " == '%s'\n", result, string);
}


/*
 * void test_memory();
 *
 * i. Perform the following operations on a point to the set_2 data block.
 * 1. print_memory(); all 32 bytes
 * 2. my_reverse(); On the first 12 bytes
 * 3. my_memset();  Set 0xEE on the 17th to the 20th bytes ([16]-[19])
 * 4. my_memmove(); Move the 6 bytes starting at the 26th byte ([25]), to
 * the 20th position ([19])
 * 5. memzero();  Bytes 12 - 16 ([11] - [15])
 * 6. my_memmove(); Move the 8 bytes starting at the 1st byte ([0]), to
 * the 9th position ([8])
 * 7. print_memory(); all 32 bytes
 *
 */
void test_memory(uint8_t *data, uint32_t size)
{
    printf("\ntest_memory :\n  ");
    print_memory(data, size);

    uint8_t *position = data;
    uint32_t length = 12;
    my_reverse(position, length);

    position = data + 16;
    length = 4;
    uint8_t value = 0xEE;
    my_memset(position, length, value);

    uint8_t *source = data + 26;
    uint8_t *destination = data + 20;
    length = 6;
    my_memmove(source, destination, length);

    position = data + 12;
    length = 5;
    my_memzero(position, length);

    source = data;
    destination = data + 9;
    length = 8;
    my_memmove(source, destination, length);

    printf("\n  ");
    print_memory(data, size);
    printf("\n");
}
