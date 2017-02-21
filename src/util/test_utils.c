#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "memory.h"
#include "data.h"

void compare_array_test(uint8_t *a, uint8_t *b, int length);

// memory.h
void test_my_memmove_1(void **state);
void test_my_memmove_2(void **state);
void test_my_memmove_3(void **state);
void test_my_memmove_4(void **state);
void test_my_memmove_5(void **state);

void test_my_memset_1(void **state);
void test_my_memset_2(void **state);
void test_my_memzero(void **state);

void test_my_reverse_1(void **state);
void test_my_reverse_2(void **state);
void test_my_reverse_3(void **state);
void test_my_reverse_4(void **state);
void test_my_reverse_5(void **state);
void test_my_reverse_6(void **state);
void test_my_reverse_7(void **state);

// data.h
void test_convert_endian32_1(void **state);
void test_convert_endian32_2(void **state);

void test_my_atoi_1(void **state);
void test_my_atoi_2(void **state);

void test_my_itoa_1(void **state);
void test_my_itoa_2(void **state);
void test_my_itoa_3(void **state);
void test_my_itoa_4(void **state);


void compare_array_test(uint8_t *a, uint8_t *b, int length)
{
    for (int i = 0; i < length; i++) {
#       if DEBUG_VERBOSE
        printf("i = %d  0x%.2x =? 0x%.2x\n", i, *(a + i), *(b + i));
#       endif
#       ifdef NDEBUG
        if (*(a + i) != *(b + i)) {
            printf("ERROR: i = %d  0x%.2x =? 0x%.2x\n", i, *(a + i), *(b + i));
        }
#       endif
        assert_int_equal(*(a + i), *(b + i));
    }
}

void test_my_memmove_1(void **state)
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
    assert_int_equal(status, 0);
    assert_memory_equal(data, expected, size);

#undef SIZE
#undef RANGE
}

void test_my_memmove_2(void **state)
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
    assert_int_equal(status, 0);
    assert_memory_equal(data, expected, size);

#undef SIZE
#undef RANGE
}

void test_my_memmove_3(void **state)
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
    assert_int_equal(status, 0);
    assert_memory_equal(data, expected, size);

#undef SIZE
#undef RANGE
}

void test_my_memmove_4(void **state)
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
    assert_int_equal(status, 0);
    assert_memory_equal(data, expected, size);

#undef SIZE
#undef RANGE
}

void test_my_memmove_5(void **state)
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
    assert_int_equal(status, 0);
    assert_memory_equal(data, expected, size);

#undef SIZE
#undef RANGE
}

void test_my_memset_1(void **state)
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
#       ifdef NDEBUG
        if (*(data + i) != expected) {
            printf("ERROR: i = %zu  0x%.2x =? 0x%.2x\n", i, *(data + i), expected);
        }
#       endif
        assert_int_equal(*(data + i), expected);
    }

    status = my_memset(NULL, size, expected);
    assert_int_not_equal(status, 0);

}

void test_my_memset_2(void **state)
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
    assert_int_equal(status, 0);
    assert_memory_equal(data, expected, size);
#undef SIZE
#undef RANGE
}

void test_my_memzero(void **state)
{
    // test zeroing an set of bytes
    size_t const size = 20;
    uint8_t data[size];
    for (size_t i = 0; i < size; i++) {
        *(data + i) = 0 + i;
    }

    uint8_t status = my_memzero(data, size);
    assert_int_equal(status, 0);
    uint8_t expected = 0;
    for (size_t i = 0; i < size; i++) {
#       ifdef NDEBUG
        if (*(data + i) != expected) {
            printf("ERROR: i = %zu  0x%.2x =? 0x%.2x\n", i, *(data + i), expected);
        }
#       endif
        assert_int_equal(*(data + i), expected);
    }

    status = my_memzero(NULL, size);
    assert_int_not_equal(status, 0);

}

void test_my_reverse_1(void **state)
{
    // test reverse returns non-zero for null pointer.
#define LENGTH 4
    int length = LENGTH;
    uint8_t data[LENGTH] = {0, 1, 2, 3};
    uint8_t expected[LENGTH] = {0, 1, 2, 3};
    uint8_t status = my_reverse(NULL, length);
    assert_int_not_equal(status, 0);
    assert_memory_equal(expected, data, length);
#undef LENGTH
}

void test_my_reverse_2(void **state)
{
    // test reverse returns reversed array for even length
#define LENGTH 4
    int length = LENGTH;
    uint8_t data[LENGTH] = {0, 1, 2, 3};
    uint8_t expected[LENGTH] = {3, 2, 1, 0};
    uint8_t status = my_reverse(data, length);
    assert_int_equal(status, 0);
    assert_memory_equal(expected, data, length);
#undef LENGTH
}

void test_my_reverse_3(void **state)
{
    // test reverse returns reversed array for odd length
#define LENGTH 5
    int length = LENGTH;
    uint8_t data[LENGTH] = {0, 1, 2, 3, 4};
    uint8_t expected[LENGTH] = {4, 3, 2, 1, 0};
    uint8_t status = my_reverse(data, length);
    assert_int_equal(status, 0);
    assert_memory_equal(expected, data, length);
#undef LENGTH
}

void test_my_reverse_4(void **state)
{
    // test reverse returns unmodified array for zero length
#define LENGTH 5
    int length = LENGTH;
    uint8_t data[LENGTH] = {0, 1, 2, 3, 4};
    uint8_t expected[LENGTH] = {0, 1, 2, 3, 4};
    uint8_t status = my_reverse(data, 0);
    assert_int_equal(status, 0);
    assert_memory_equal(expected, data, length);
#undef LENGTH
}

void test_my_reverse_5(void **state)
{
    // test reverse does not affect memory beyond specified length.
#define LENGTH 5
    int length = LENGTH;
    uint8_t data[LENGTH] = {0, 1, 2, 3, 4};
    uint8_t expected[LENGTH] = {3, 2, 1, 0, 4};
    uint8_t status = my_reverse(data, 4);
    assert_int_equal(status, 0);
    assert_memory_equal(expected, data, length);
#undef LENGTH
}

void test_my_reverse_6(void **state)
{
    // test reverse does not affect memory before the specified start
#define LENGTH 5
    int length = LENGTH;
    uint8_t data[LENGTH] = {0, 1, 2, 3, 4};
    uint8_t expected[LENGTH] = {0, 4, 3, 2, 1};
    uint8_t status = my_reverse(data + 1, 4);
    assert_int_equal(status, 0);
    assert_memory_equal(expected, data, length);
#undef LENGTH
}

void test_my_reverse_7(void **state)
{
    // test reversing the entire array, even length
    size_t const size = 20;
    uint8_t data[size];
    uint8_t expected[size];
    for (size_t i = 0; i < size; i++) {
        *(data + i) = i;
        *(expected + i) = size - 1 - i;
    }

    uint8_t status = my_reverse(data, size);
    assert_int_equal(status, 0);
    assert_memory_equal(expected, data, size);

}

void test_convert_endian32_1(void **state)
{
    // test convert endian by round tripping data, single 8-byte int
    uint32_t const length = 1;
    uint32_t data = 0x01234567;
    uint32_t expected = 0x67452301;
    size_t num_bytes = sizeof(data);

    // note: just calling the wrapper to avoid exposing convert_endian
    uint8_t status = big_to_little32(&data, length);
    assert_int_equal(status, 0);
    assert_memory_equal((uint8_t *)&expected, (uint8_t *)&data, length * num_bytes);

    uint32_t expected_orig = 0x01234567;
    status = little_to_big32(&data, length);
    assert_int_equal(status, 0);
    assert_memory_equal((uint8_t *)&expected_orig, (uint8_t *)&data,
                       length * num_bytes);

}

void test_convert_endian32_2(void **state)
{
    // test convert endian, array of 4 8-byte ints
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
    assert_int_equal(status, 0);
    assert_memory_equal((uint8_t *)expected, (uint8_t *)data, length * num_bytes);

    status = big_to_little32(data, length);
    assert_int_equal(status, 0);
    assert_memory_equal((uint8_t *)expected_orig, (uint8_t *)data,
                       length * num_bytes);

}

void test_my_atoi_1(void **state)
{
    // test converting a null terminated string with leading whitespace.
#define LENGTH 10
    int8_t string[LENGTH] = "     1002\0";
    uint32_t expected = 1002;
    uint32_t received = my_atoi(string);
#   ifdef NDEBUG
    if (received != expected) {
        printf("ERROR: 0x%.2x =? 0x%.2x\n", received, expected);
    }
#   endif
    assert_int_equal(expected, received);
#undef LENGTH
}

void test_my_atoi_2(void **state)
{
    // test converting a negative number as a null terminated string
#define LENGTH 10
    int8_t string[LENGTH] = " -1537902\0";
    uint32_t expected = -1537902;
    uint32_t received = my_atoi(string);
#   ifdef NDEBUG
    if (received != expected) {
        printf("ERROR: 0x%.2x =? 0x%.2x\n", received, expected);
    }
#   endif
    assert_int_equal(expected, received);
#undef LENGTH
}

void test_my_itoa_1(void **state)
{
    // test converting an integer base 10 to a string
#define LENGTH 33
    uint32_t length = LENGTH;
    int8_t string[LENGTH];
    int8_t expected[LENGTH];
    for (uint8_t i = 0; i < length; i++) {
        string[i] = '\0';
        expected[i] = string[i];
    }
    my_memmove((uint8_t *)"1023", (uint8_t *)expected, 4);
    uint32_t data = 1023;
    uint32_t base = 10;
    int8_t *junk = my_itoa(string, data, base);

    assert_ptr_equal(junk, string);
    assert_memory_equal((uint8_t *)string, (uint8_t *)expected, length);
#undef LENGTH
}

void test_my_itoa_2(void **state)
{
    // test converting an negative integer base 10 to a string
#define LENGTH 33
    uint32_t length = LENGTH;
    int8_t string[LENGTH];
    int8_t expected[LENGTH];
    for (uint8_t i = 0; i < length; i++) {
        string[i] = '\0';
        expected[i] = string[i];
    }
    my_memmove((uint8_t *)"-10325476", (uint8_t *)expected, 9);
    uint32_t data = -10325476;
    uint32_t base = 10;
    int8_t *junk = my_itoa(string, data, base);

    assert_ptr_equal(junk, string);
    assert_memory_equal((uint8_t *)string, (uint8_t *)expected, length);
#undef LENGTH
}


void test_my_itoa_3(void **state)
{
    // test converting an integer base 2 to a string
#define LENGTH 33
    uint32_t length = LENGTH;
    int8_t string[LENGTH];
    int8_t expected[LENGTH];
    for (uint8_t i = 0; i < length; i++) {
        string[i] = '\0';
        expected[i] = string[i];
    }
    my_memmove((uint8_t *)"1111110101", (uint8_t *)expected, 10);

    uint32_t data = 1013;
    uint32_t base = 2;
    int8_t *junk = my_itoa(string, data, base);

    assert_ptr_equal(junk, string);
    assert_memory_equal((uint8_t *)string, (uint8_t *)expected, length);
#undef LENGTH
}

void test_my_itoa_4(void **state)
{
    // test converting a negative integer base 16 to a string
#define LENGTH 33
    uint32_t length = LENGTH;
    int8_t string[LENGTH];
    int8_t expected[LENGTH];
    for (uint8_t i = 0; i < length; i++) {
        string[i] = '\0';
        expected[i] = string[i];
    }
    my_memmove((uint8_t *)"-9d8de4", (uint8_t *)expected, 7);

    uint32_t data = -10325476;
    uint32_t base = 16;
    int8_t *junk = my_itoa(string, data, base);

    assert_ptr_equal(junk, string);
    assert_memory_equal((uint8_t *)string, (uint8_t *)expected, length);
#undef LENGTH
}

int main(int argc, char **argv)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_my_memmove_1),
        cmocka_unit_test(test_my_memmove_2),
        cmocka_unit_test(test_my_memmove_3),
        cmocka_unit_test(test_my_memmove_4),
        cmocka_unit_test(test_my_memmove_5),
        cmocka_unit_test(test_my_memset_1),
        cmocka_unit_test(test_my_memset_2),
        cmocka_unit_test(test_my_memzero),
        cmocka_unit_test(test_my_reverse_1),
        cmocka_unit_test(test_my_reverse_2),
        cmocka_unit_test(test_my_reverse_3),
        cmocka_unit_test(test_my_reverse_4),
        cmocka_unit_test(test_my_reverse_5),
        cmocka_unit_test(test_my_reverse_6),
        cmocka_unit_test(test_my_reverse_7),
        cmocka_unit_test(test_convert_endian32_1),
        cmocka_unit_test(test_convert_endian32_2),
        cmocka_unit_test(test_my_atoi_1),
        cmocka_unit_test(test_my_atoi_2),
        cmocka_unit_test(test_my_itoa_1),
        cmocka_unit_test(test_my_itoa_2),
        cmocka_unit_test(test_my_itoa_3),
        cmocka_unit_test(test_my_itoa_4),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}

