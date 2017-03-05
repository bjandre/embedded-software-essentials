#include <stdio.h>

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "memory.h"

#include "test_memory.h"

void test_my_memmove_null_ptrs(void **state);
void test_my_memmove_src_dest_complete_overlap(void **state);
void test_my_memmove_no_overlap_src_dest(void **state);
void test_my_memmove_no_overlap_dest_src(void **state);
void test_my_memmove_overlap_src_in_dest(void **state);
void test_my_memmove_overlap_dest_in_src(void **state);

void test_my_memset_null_ptrs(void **state);
void test_my_memset_set_entire_array(void **state);
void test_my_memset_set_subarray(void **state);

void test_my_memzero_null_ptrs(void **state);
void test_my_memzero_set_entire_array(void **state);

void test_my_reverse_null_ptr(void **state);
void test_my_reverse_even_length(void **state);
void test_my_reverse_odd_length(void **state);
void test_my_reverse_zero_length(void **state);
void test_my_reverse_subarray_after(void **state);
void test_my_reverse_subarray_before(void **state);
void test_my_reverse_all_characters(void **state);

int suite_memory(void)
{
    const struct CMUnitTest memory_tests[] = {
        cmocka_unit_test(test_my_memmove_null_ptrs),
        cmocka_unit_test(test_my_memmove_src_dest_complete_overlap),
        cmocka_unit_test(test_my_memmove_no_overlap_src_dest),
        cmocka_unit_test(test_my_memmove_no_overlap_dest_src),
        cmocka_unit_test(test_my_memmove_overlap_src_in_dest),
        cmocka_unit_test(test_my_memmove_overlap_dest_in_src),
        cmocka_unit_test(test_my_memset_null_ptrs),
        cmocka_unit_test(test_my_memset_set_entire_array),
        cmocka_unit_test(test_my_memset_set_subarray),
        cmocka_unit_test(test_my_memzero_null_ptrs),
        cmocka_unit_test(test_my_memzero_set_entire_array),
        cmocka_unit_test(test_my_reverse_null_ptr),
        cmocka_unit_test(test_my_reverse_even_length),
        cmocka_unit_test(test_my_reverse_odd_length),
        cmocka_unit_test(test_my_reverse_zero_length),
        cmocka_unit_test(test_my_reverse_subarray_after),
        cmocka_unit_test(test_my_reverse_subarray_before),
        cmocka_unit_test(test_my_reverse_all_characters),
    };

    return cmocka_run_group_tests(memory_tests, NULL, NULL);
}

void test_my_memmove_null_ptrs(void **state)
{
    // test return status when null pointers are passed in
#define SIZE 16

    size_t const range = 4;
    uint8_t data[SIZE] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    uint8_t *source = data + range;

    MemStatus status = my_memmove(NULL, source, range);
    assert_int_equal(status, MemStatus_ERROR_NULL);
    status = my_memmove(source, NULL, range);
    assert_int_equal(status, MemStatus_ERROR_NULL);

#undef SIZE
}

void test_my_memmove_src_dest_complete_overlap(void **state)
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
    MemStatus status = my_memmove(source, destination, range);
    assert_int_equal(status, MemStatus_SUCCESS);
    assert_memory_equal(data, expected, size);

#undef SIZE
#undef RANGE
}

void test_my_memmove_no_overlap_src_dest(void **state)
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
    MemStatus status = my_memmove(source, destination, range);
    assert_int_equal(status, MemStatus_SUCCESS);
    assert_memory_equal(data, expected, size);

#undef SIZE
#undef RANGE
}

void test_my_memmove_no_overlap_dest_src(void **state)
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
    MemStatus status = my_memmove(source, destination, range);
    assert_int_equal(status, MemStatus_SUCCESS);
    assert_memory_equal(data, expected, size);

#undef SIZE
#undef RANGE
}

void test_my_memmove_overlap_src_in_dest(void **state)
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
    MemStatus status = my_memmove(source, destination, range);
    assert_int_equal(status, MemStatus_SUCCESS);
    assert_memory_equal(data, expected, size);

#undef SIZE
#undef RANGE
}

void test_my_memmove_overlap_dest_in_src(void **state)
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
    MemStatus status = my_memmove(source, destination, range);
    assert_int_equal(status, MemStatus_SUCCESS);
    assert_memory_equal(data, expected, size);

#undef SIZE
#undef RANGE
}

void test_my_memset_null_ptrs(void **state)
{
    // test return error when passed null pointer
    size_t const size = 20;
    uint8_t expected = 0x23;

    MemStatus status = my_memset(NULL, size, expected);
    assert_int_equal(status, MemStatus_ERROR_NULL);
}

void test_my_memset_set_entire_array(void **state)
{
    // test setting entire array
    size_t const size = 20;
    uint8_t data[size];
    for (size_t i = 0; i < size; i++) {
        *(data + i) = i;
    }
    uint8_t expected = 0x23;
    MemStatus status = my_memset(data, size, expected);
    assert_int_equal(status, MemStatus_SUCCESS);

    for (size_t i = 0; i < size; i++) {
#       ifdef NDEBUG
        if (*(data + i) != expected) {
            printf("ERROR: i = %zu  0x%.2x =? 0x%.2x\n", i, *(data + i), expected);
        }
#       endif
        assert_int_equal(*(data + i), expected);
    }
}

void test_my_memset_set_subarray(void **state)
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
    MemStatus status = my_memset(data + 2, range, value);
    assert_int_equal(status, MemStatus_SUCCESS);
    assert_memory_equal(data, expected, size);
#undef SIZE
#undef RANGE
}

void test_my_memzero_null_ptrs(void **state)
{
    // test error status when passed null pointer
    uint32_t size = 10;
    MemStatus status = my_memzero(NULL, size);
    assert_int_equal(status, MemStatus_ERROR_NULL);

}

void test_my_memzero_set_entire_array(void **state)
{
    // test zeroing an set of bytes
    size_t const size = 20;
    uint8_t data[size];
    for (size_t i = 0; i < size; i++) {
        *(data + i) = i;
    }

    MemStatus status = my_memzero(data, size);
    assert_int_equal(status, MemStatus_SUCCESS);
    uint8_t expected = 0;
    for (size_t i = 0; i < size; i++) {
#       ifdef NDEBUG
        if (*(data + i) != expected) {
            printf("ERROR: i = %zu  0x%.2x =? 0x%.2x\n", i, *(data + i), expected);
        }
#       endif
        assert_int_equal(*(data + i), expected);
    }
}

void test_my_reverse_null_ptr(void **state)
{
    // test reverse returns error for null pointer.
    int length = 4;
    MemStatus status = my_reverse(NULL, length);
    assert_int_equal(status, MemStatus_ERROR_NULL);
}

void test_my_reverse_even_length(void **state)
{
    // test reverse returns reversed array for even length
#define LENGTH 4
    int length = LENGTH;
    uint8_t data[LENGTH] = {0, 1, 2, 3};
    uint8_t expected[LENGTH] = {3, 2, 1, 0};
    MemStatus status = my_reverse(data, length);
    assert_int_equal(status, MemStatus_SUCCESS);
    assert_memory_equal(expected, data, length);
#undef LENGTH
}

void test_my_reverse_odd_length(void **state)
{
    // test reverse returns reversed array for odd length
#define LENGTH 5
    int length = LENGTH;
    uint8_t data[LENGTH] = {0, 1, 2, 3, 4};
    uint8_t expected[LENGTH] = {4, 3, 2, 1, 0};
    MemStatus status = my_reverse(data, length);
    assert_int_equal(status, MemStatus_SUCCESS);
    assert_memory_equal(expected, data, length);
#undef LENGTH
}

void test_my_reverse_zero_length(void **state)
{
    // test reverse returns unmodified array for zero length
#define LENGTH 5
    int length = LENGTH;
    uint8_t data[LENGTH] = {0, 1, 2, 3, 4};
    uint8_t expected[LENGTH] = {0, 1, 2, 3, 4};
    MemStatus status = my_reverse(data, 0);
    assert_int_equal(status, MemStatus_SUCCESS);
    assert_memory_equal(expected, data, length);
#undef LENGTH
}

void test_my_reverse_subarray_after(void **state)
{
    // test reverse does not affect memory beyond specified length.
#define LENGTH 5
    int length = LENGTH;
    uint8_t data[LENGTH] = {0, 1, 2, 3, 4};
    uint8_t expected[LENGTH] = {3, 2, 1, 0, 4};
    MemStatus status = my_reverse(data, 4);
    assert_int_equal(status, MemStatus_SUCCESS);
    assert_memory_equal(expected, data, length);
#undef LENGTH
}

void test_my_reverse_subarray_before(void **state)
{
    // test reverse does not affect memory before the specified start
#define LENGTH 5
    int length = LENGTH;
    uint8_t data[LENGTH] = {0, 1, 2, 3, 4};
    uint8_t expected[LENGTH] = {0, 4, 3, 2, 1};
    MemStatus status = my_reverse(data + 1, 4);
    assert_int_equal(status, MemStatus_SUCCESS);
    assert_memory_equal(expected, data, length);
#undef LENGTH
}

void test_my_reverse_all_characters(void **state)
{
    // test reversing the entire array, even length
    size_t const size = 256;
    uint8_t data[size];
    uint8_t expected[size];
    for (size_t i = 0; i < size; i++) {
        *(data + i) = i;
        *(expected + i) = size - 1 - i;
    }

    MemStatus status = my_reverse(data, size);
    assert_int_equal(status, MemStatus_SUCCESS);
    assert_memory_equal(expected, data, size);

}
