#include <stdint.h>
#include <stdlib.h>

#include "data.h"
#include "memory.h"
#include "project_1.h"

/*
 * void project_1_report()
 *
 * i. set_1 : Create an array of 32 bytes (unsigned). Initialize this array to increasing
 * hex numbers like: 0x0123456789ABCDEF0123456789ABCDEF
 * ii. set_2 : Create another array of 32 bytes (unsigned). Initialize this array to an
 * increasing integer count of 1 ,2, 3, 4, 5, 6, 7, 8, 9, 10, 11, ...., 31.
 * iii. set_3 : Create another array of 32 bytes (unsigned). Initialize this array to an
 * increasing integer count of “abcdefghijklmnopqrstuvwxyzABCDEF”
 * iv. This should call the functions below in order.
 */

size_t const set_size = 32;

void project_1_report()
{
    uint8_t set_1[set_size];
    *set_1 = 0; // 0x0123456789ABCDEF0123456789ABCDEF;

    uint8_t set_2[set_size];
    for (uint8_t i = 0; i < set_size; i++) {
        *(set_2 + i) = i;
    }

    uint8_t set_3[set_size];
    for (uint8_t i = 0; i < set_size; i++) {
        *(set_3 + i) = 'a' + i;
    }

    test_data1(set_1);
    test_data2(set_2);
    test_data3();
    test_memory(set_2);
}

/*
 * void test_data1();
 *
 *i. Show your data functions work by calling the following functions in this
 *order with the first array, set_1 : 1. print_memory() 2. big_to_little32()
 *3. print_memory() 4. little_to_big32() 5. print_memory()
 *
 */
void test_data1(uint8_t *data, uint32_t size)
{
    print_memory(data, size);
    big_to_little32((uint32_t *)data, size);
    print_memory(data, size);
    little_to_big32((uint32_t *)data, size);
    print_memory(data, size);
}

/*
 * void test_data2();
 *
 * i. Show your data functions work by calling the following functions in this
 * order with the second array, set_2 : my_itoa() & then print_memory()
 *
 */
void test_data2(uint8_t *data, uint32_t size)
{
    int8_t string[set_size];
    int32_t base = 2;
    my_itoa(string, *(int32_t *)data, base);
    print_memory(data, size);
}

/*
 * void test_data3();
 *
 * i. Show your data functions work by calling the following functions in this
 * order with the second array, set_3 : my_atoi() & then print_memory()
 *
 */
void test_data3()
{

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
void test_memory()
{
//    print_memory(data);
//    my_reverse(data);
//    my_memset(data);
//    my_memmove(data);
//    my_memzero(data);
//    my_memmove(data);
//    print_memory(data);
}
