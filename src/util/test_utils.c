#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "memory.h"
#include "data.h"

void compare_array_test(uint8_t *a, uint8_t *b, int length);

uint8_t test_my_memmove_1(void);
uint8_t test_my_memmove_2(void);
uint8_t test_my_memmove_3(void);
uint8_t test_my_memmove_4(void);
uint8_t test_my_memmove_5(void);

uint8_t test_my_memset_1(void);
uint8_t test_my_memset_2(void);
uint8_t test_my_memzero(void);

uint8_t test_my_reverse_1(void);
uint8_t test_my_reverse_2(void);
uint8_t test_my_reverse_3(void);
uint8_t test_my_reverse_4(void);
uint8_t test_my_reverse_5(void);
uint8_t test_my_reverse_6(void);
uint8_t test_my_reverse_7(void);

uint8_t test_convert_endian32_1(void);
uint8_t test_convert_endian32_2(void);

int main(int argc, char **argv)
{
    printf("test utils : ");
    uint8_t success = 0;
    success += test_my_memmove_1();
    success += test_my_memmove_2();
    success += test_my_memmove_3();
    success += test_my_memmove_4();
    success += test_my_memmove_5();

    success += test_my_memset_1();
    success += test_my_memset_2();

    success += test_my_memzero();

    success += test_my_reverse_1();
    success += test_my_reverse_2();
    success += test_my_reverse_3();
    success += test_my_reverse_4();
    success += test_my_reverse_5();
    success += test_my_reverse_6();
    success += test_my_reverse_7();

    success += test_convert_endian32_1();
    success += test_convert_endian32_2();

    printf(" : %hhu tests PASS\n", success);
}

void compare_array_test(uint8_t *a, uint8_t *b, int length)
{
    for (int i = 0; i < length; i++) {
        assert(*(a + i) == *(b + i));
    }
}

uint8_t test_my_memmove_1()
{
    // test complete overlap
#define SIZE 16
#define RANGE 4

    size_t const size = SIZE;
    size_t const range = RANGE;
    uint8_t data[SIZE] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    uint8_t *source = data + range;
    uint8_t *destination = data + range;

    uint8_t expected[SIZE] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    uint8_t status = my_memmove(source, destination, range);
    assert(status == 0);
    compare_array_test(data, expected, size);

#undef SIZE
#undef RANGE
    printf(".");
    return 1;
}

uint8_t test_my_memmove_2()
{
    // test no overlap, source before destestination
#define SIZE 16
#define RANGE 4

    size_t const size = SIZE;
    size_t const range = RANGE;
    uint8_t data[SIZE] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    uint8_t *source  = data + range / 2;
    uint8_t *destination = data + 2 * range;

    uint8_t expected[SIZE] = {0, 1, 2, 3, 4, 5, 6, 7, 2, 3, 4, 5, 12, 13, 14, 15};
    uint8_t status = my_memmove(source, destination, range);
    assert(status == 0);
    compare_array_test(data, expected, size);

#undef SIZE
#undef RANGE
    printf(".");
    return 1;
}

uint8_t test_my_memmove_3()
{
    // test no overlap, destination before source
#define SIZE 16
#define RANGE 4

    size_t const size = SIZE;
    size_t const range = RANGE;
    uint8_t data[SIZE] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    uint8_t *source = data + 2 * range;
    uint8_t *destination = data + range / 2;

    uint8_t expected[SIZE] = {0, 1, 8, 9, 10, 11, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    uint8_t status = my_memmove(source, destination, range);
    assert(status == 0);
    compare_array_test(data, expected, size);

#undef SIZE
#undef RANGE
    printf(".");
    return 1;
}

uint8_t test_my_memmove_4()
{
    // test overlap, destination before source, copy from begining of source
#define SIZE 16
#define RANGE 4

    size_t const size = SIZE;
    size_t const range = RANGE;
    uint8_t data[SIZE] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    uint8_t *source = data + range;
    uint8_t *destination = data + range / 2;

    uint8_t expected[SIZE] = {0, 1, 4, 5, 6, 7, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    uint8_t status = my_memmove(source, destination, range);
    assert(status == 0);
    compare_array_test(data, expected, size);

#undef SIZE
#undef RANGE
    printf(".");
    return 1;
}

uint8_t test_my_memmove_5()
{
    // test overlap, source before destination, copy from end of source
#define SIZE 16
#define RANGE 4

    size_t const size = SIZE;
    size_t const range = RANGE;
    uint8_t data[SIZE] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    uint8_t *source = data + range / 2;
    uint8_t *destination = data + range;

    uint8_t expected[SIZE] = {0, 1, 2, 3, 2, 3, 4, 5, 8, 9, 10, 11, 12, 13, 14, 15};
    uint8_t status = my_memmove(source, destination, range);
    assert(status == 0);
    compare_array_test(data, expected, size);

#undef SIZE
#undef RANGE
    printf(".");
    return 1;
}

uint8_t test_my_memset_1()
{
    // test setting entire array
    size_t const size = 20;
    uint8_t data[size];
    for (size_t i = 0; i < size; i++) {
        *(data + i) = 0 + i;
    }
    uint8_t expected = 0x23;
    uint8_t status = my_memset(data, size, expected);

    for (size_t i = 0; i < size; i++) {
        assert(*(data + i) == expected);
    }

    status = my_memset(NULL, size, expected);
    assert(status != 0);
    printf(".");
    return 1;
}

uint8_t test_my_memset_2()
{
    // test a subset of an array to ensure we are not changing values outside
    // the specified range.
#define SIZE 8
#define RANGE 4

    size_t const size = SIZE;
    size_t const range = RANGE;
    uint8_t data[SIZE] = {0, 1, 2, 3, 4, 5, 6, 7};

    uint8_t value = 0x34;
    uint8_t expected[SIZE] = {0, 1, value, value, value, value, 6, 7};
    uint8_t status = my_memset(data + 2, range, value);
    assert(status == 0);
    for (size_t i = 0; i < size; i++) {
        assert(*(data + i) == *(expected + i));
    }
#undef SIZE
#undef RANGE
    printf(".");
    return 1;
}

uint8_t test_my_memzero()
{
    size_t const size = 20;
    uint8_t data[size];
    for (size_t i = 0; i < size; i++) {
        *(data + i) = 0 + i;
    }

    uint8_t status = my_memzero(data, size);
    assert(status == 0);
    uint8_t expected = 0;
    for (size_t i = 0; i < size; i++) {
        assert(*(data + i) == expected);
    }

    status = my_memzero(NULL, size);
    assert(status != 0);
    printf(".");
    return 1;
}

uint8_t test_my_reverse_1(void)
{
    // test reverse returns non-zero for null pointer.
#define LENGTH 4
    int length = LENGTH;
    uint8_t data[LENGTH] = {0, 1, 2, 3};
    uint8_t expected[LENGTH] = {0, 1, 2, 3};
    uint8_t status = my_reverse(NULL, length);
    assert(status != 0);
    compare_array_test(expected, data, length);
#undef LENGTH
    printf(".");
    return 1;
}

uint8_t test_my_reverse_2(void)
{
    // test reverse returns reversed array for even length
#define LENGTH 4
    int length = LENGTH;
    uint8_t data[LENGTH] = {0, 1, 2, 3};
    uint8_t expected[LENGTH] = {3, 2, 1, 0};
    uint8_t status = my_reverse(data, length);
    assert(status == 0);
    compare_array_test(expected, data, length);
#undef LENGTH
    printf(".");
    return 1;
}

uint8_t test_my_reverse_3(void)
{
    // test reverse returns reversed array for odd length
#define LENGTH 5
    int length = LENGTH;
    uint8_t data[LENGTH] = {0, 1, 2, 3, 4};
    uint8_t expected[LENGTH] = {4, 3, 2, 1, 0};
    uint8_t status = my_reverse(data, length);
    assert(status == 0);
    compare_array_test(expected, data, length);
#undef LENGTH
    printf(".");
    return 1;
}

uint8_t test_my_reverse_4(void)
{
    // test reverse returns unmodified array for zero length
#define LENGTH 5
    int length = LENGTH;
    uint8_t data[LENGTH] = {0, 1, 2, 3, 4};
    uint8_t expected[LENGTH] = {0, 1, 2, 3, 4};
    uint8_t status = my_reverse(data, 0);
    assert(status == 0);
    compare_array_test(expected, data, length);
#undef LENGTH
    printf(".");
    return 1;
}

uint8_t test_my_reverse_5(void)
{
    // test reverse does not affect memory beyond specified length.
#define LENGTH 5
    int length = LENGTH;
    uint8_t data[LENGTH] = {0, 1, 2, 3, 4};
    uint8_t expected[LENGTH] = {3, 2, 1, 0, 4};
    uint8_t status = my_reverse(data, 4);
    assert(status == 0);
    compare_array_test(expected, data, length);
#undef LENGTH
    printf(".");
    return 1;
}

uint8_t test_my_reverse_6(void)
{
    // test reverse does not affect memory before the specified start
#define LENGTH 5
    int length = LENGTH;
    uint8_t data[LENGTH] = {0, 1, 2, 3, 4};
    uint8_t expected[LENGTH] = {0, 4, 3, 2, 1};
    uint8_t status = my_reverse(data + 1, 4);
    assert(status == 0);
    compare_array_test(expected, data, length);
#undef LENGTH
    printf(".");
    return 1;
}

uint8_t test_my_reverse_7()
{
    size_t const size = 20;
    uint8_t data[size];
    uint8_t expected[size];
    for (size_t i = 0; i < size; i++) {
        *(data + i) = i;
        *(expected + i) = size - 1 - i;
    }

    uint8_t status = my_reverse(data, size);
    assert(status == 0);
    compare_array_test(expected, data, size);
    printf(".");
    return 1;
}

uint8_t test_convert_endian32_1()
{
    uint32_t const length = 1;
    uint32_t data = 0x01234567;
    uint32_t expected = 0x67452301;
    size_t num_bytes = sizeof(data);

    // note: just calling the wrapper to avoid exposing convert_endian
    uint8_t status = big_to_little32(&data, length);
    assert(status == 0);
    compare_array_test((uint8_t *)&expected, (uint8_t *)&data, length * num_bytes);

    uint32_t expected_orig = 0x01234567;
    status = little_to_big32(&data, length);
    assert(status == 0);
    compare_array_test((uint8_t *)&expected_orig, (uint8_t *)&data,
                       length * num_bytes);
    printf(".");
    return 1;
}

uint8_t test_convert_endian32_2()
{
    uint32_t const length = 4;
    uint32_t data[length];
    uint32_t expected[length];
    size_t num_bytes = sizeof(*data);
    *data = 0x01234567;
    *expected = 0x67452301;
    *(data + 1) = 0x89abcdef;
    *(expected + 1) = 0xefcdab89;
    *(data + 2) = 0x01020304;
    *(expected + 2) = 0x04030201;
    *(data + 3) = 0x05060708;
    *(expected + 3) = 0x08070605;

    uint32_t expected_orig[length];
    for (uint32_t i = 0; i < length; i++) {
        *(expected_orig + i) = *(data + i);
    }

    // note: just calling the wrapper to avoid exposing convert_endian
    uint8_t status = little_to_big32(data, length);
    assert(status == 0);
    compare_array_test((uint8_t *)expected, (uint8_t *)data, length * num_bytes);

    status = big_to_little32(data, length);
    assert(status == 0);
    compare_array_test((uint8_t *)expected_orig, (uint8_t *)data,
                       length * num_bytes);
    printf(".");
    return 1;
}

