
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include <stdio.h>

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "memory-common.h"
#include "memory-cpu.h"

#include "test_memory.h"

void test_memmove_cpu_null_ptrs(void **state);
void test_memmove_cpu_src_dest_complete_overlap(void **state);
void test_memmove_cpu_no_overlap_src_dest(void **state);
void test_memmove_cpu_no_overlap_dest_src(void **state);
void test_memmove_cpu_overlap_src_in_dest(void **state);
void test_memmove_cpu_overlap_dest_in_src(void **state);

void test_memset_cpu_null_ptrs(void **state);
void test_memset_cpu_set_entire_array(void **state);
void test_memset_cpu_set_subarray(void **state);

void test_my_memzero_null_ptrs(void **state);
void test_my_memzero_set_entire_array(void **state);

void test_my_reverse_null_ptr(void **state);
void test_my_reverse_even_length(void **state);
void test_my_reverse_odd_length(void **state);
void test_my_reverse_zero_length(void **state);
void test_my_reverse_subarray_after(void **state);
void test_my_reverse_subarray_before(void **state);
void test_my_reverse_all_characters(void **state);

/**
   Test suite for memory utilities
 */
int suite_memory(void)
{
    const struct CMUnitTest memory_tests[] = {
        cmocka_unit_test(test_memmove_cpu_null_ptrs),
        cmocka_unit_test(test_memmove_cpu_src_dest_complete_overlap),
        cmocka_unit_test(test_memmove_cpu_no_overlap_src_dest),
        cmocka_unit_test(test_memmove_cpu_no_overlap_dest_src),
        cmocka_unit_test(test_memmove_cpu_overlap_src_in_dest),
        cmocka_unit_test(test_memmove_cpu_overlap_dest_in_src),
        cmocka_unit_test(test_memset_cpu_null_ptrs),
        cmocka_unit_test(test_memset_cpu_set_entire_array),
        cmocka_unit_test(test_memset_cpu_set_subarray),
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

void test_memmove_cpu_null_ptrs(void **state)
{
    /**
       test return status indicates error when null pointers are passed in
    */
#define SIZE 16 /**< @hideinitializer */

    size_t const range = 4;
    uint8_t data[SIZE] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    uint8_t *source = data + range;

    MemStatus status = memmove_cpu(NULL, source, range);
    assert_int_equal(status, MemStatus_Null_Pointer);
    status = memmove_cpu(source, NULL, range);
    assert_int_equal(status, MemStatus_Null_Pointer);

#undef SIZE
}

void test_memmove_cpu_src_dest_complete_overlap(void **state)
{
    /**
       test memmove with complete overlap of src and dest
     */
#define SIZE 16  /**< @hideinitializer */
#define RANGE 4  /**< @hideinitializer */

    size_t const size = SIZE;
    size_t const range = RANGE;
    uint8_t data[SIZE] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    uint8_t *source = data + range;
    uint8_t *destination = data + range;

    uint8_t expected[SIZE] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    MemStatus status = memmove_cpu(source, destination, range);
    assert_int_equal(status, MemStatus_Success);
    assert_memory_equal(data, expected, size);

#undef SIZE
#undef RANGE
}

void test_memmove_cpu_no_overlap_src_dest(void **state)
{
    /**
        test memmove with no overlap between source and destination, source
        address comes before destestination
    */
#define SIZE 16  /**< @hideinitializer */
#define RANGE 4  /**< @hideinitializer */

    size_t const size = SIZE;
    size_t const range = RANGE;
    uint8_t data[SIZE] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    uint8_t *source  = data + range / 2;
    uint8_t *destination = data + 2 * range;

    uint8_t expected[SIZE] = {0, 1, 2, 3, 4, 5, 6, 7, 2, 3, 4, 5, 12, 13, 14, 15};
    MemStatus status = memmove_cpu(source, destination, range);
    assert_int_equal(status, MemStatus_Success);
    assert_memory_equal(data, expected, size);

#undef SIZE
#undef RANGE
}

void test_memmove_cpu_no_overlap_dest_src(void **state)
{
    /**
       test memmove with no overlap between source and destination, destination
       address before source
    */
#define SIZE 16 /**< @hideinitializer */
#define RANGE 4 /**< @hideinitializer */

    size_t const size = SIZE;
    size_t const range = RANGE;
    uint8_t data[SIZE] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    uint8_t *source = data + 2 * range;
    uint8_t *destination = data + range / 2;

    uint8_t expected[SIZE] = {0, 1, 8, 9, 10, 11, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    MemStatus status = memmove_cpu(source, destination, range);
    assert_int_equal(status, MemStatus_Success);
    assert_memory_equal(data, expected, size);

#undef SIZE
#undef RANGE
}

void test_memmove_cpu_overlap_src_in_dest(void **state)
{
    /**
       test overlap source and destination, destination before source, copy from
       begining of source
    */
#define SIZE 16 /**< @hideinitializer */
#define RANGE 4 /**< @hideinitializer */

    size_t const size = SIZE;
    size_t const range = RANGE;
    uint8_t data[SIZE] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    uint8_t *source = data + range;
    uint8_t *destination = data + range / 2;

    uint8_t expected[SIZE] = {0, 1, 4, 5, 6, 7, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    MemStatus status = memmove_cpu(source, destination, range);
    assert_int_equal(status, MemStatus_Success);
    assert_memory_equal(data, expected, size);

#undef SIZE
#undef RANGE
}

void test_memmove_cpu_overlap_dest_in_src(void **state)
{
    /**
        test overlap of source and destination, source before destination, copy
        from end of source
    */
#define SIZE 16 /**< @hideinitializer */
#define RANGE 4 /**< @hideinitializer */

    size_t const size = SIZE;
    size_t const range = RANGE;
    uint8_t data[SIZE] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    uint8_t *source = data + range / 2;
    uint8_t *destination = data + range;

    uint8_t expected[SIZE] = {0, 1, 2, 3, 2, 3, 4, 5, 8, 9, 10, 11, 12, 13, 14, 15};
    MemStatus status = memmove_cpu(source, destination, range);
    assert_int_equal(status, MemStatus_Success);
    assert_memory_equal(data, expected, size);

#undef SIZE
#undef RANGE
}

void test_memset_cpu_null_ptrs(void **state)
{
    /**
       test return error when passed null pointer
    */
    size_t const size = 20;
    uint8_t expected = 0x23;

    MemStatus status = memset_cpu(NULL, size, expected);
    assert_int_equal(status, MemStatus_Null_Pointer);
}

void test_memset_cpu_set_entire_array(void **state)
{
    /**
       test setting entire array to the same value
    */
    size_t const size = 20;
    uint8_t data[size];
    for (size_t i = 0; i < size; i++) {
        *(data + i) = i;
    }
    uint8_t expected = 0x23;
    MemStatus status = memset_cpu(data, size, expected);
    assert_int_equal(status, MemStatus_Success);

    for (size_t i = 0; i < size; i++) {
#       ifdef NDEBUG
        if (*(data + i) != expected) {
            printf("ERROR: i = %zu  0x%.2x =? 0x%.2x\n", i, *(data + i), expected);
        }
#       endif
        assert_int_equal(*(data + i), expected);
    }
}

void test_memset_cpu_set_subarray(void **state)
{
    /**
       test a subset of an array to ensure we are not changing values outside
       the specified range.
    */
#define SIZE 8 /**< @hideinitializer */
#define RANGE 4 /**< @hideinitializer */

    size_t const size = SIZE;
    size_t const range = RANGE;
    uint8_t data[SIZE] = {0, 1, 2, 3, 4, 5, 6, 7};

    uint8_t value = 0x34;
    uint8_t expected[SIZE] = {0, 1, value, value, value, value, 6, 7};
    MemStatus status = memset_cpu(data + 2, range, value);
    assert_int_equal(status, MemStatus_Success);
    assert_memory_equal(data, expected, size);
#undef SIZE
#undef RANGE
}

void test_my_memzero_null_ptrs(void **state)
{
    /**
       test error status when passed null pointer
    */
    uint32_t size = 10;
    MemStatus status = my_memzero(NULL, size);
    assert_int_equal(status, MemStatus_Null_Pointer);

}

void test_my_memzero_set_entire_array(void **state)
{
    /**
       test zeroing an set of bytes
    */
    size_t const size = 20;
    uint8_t data[size];
    for (size_t i = 0; i < size; i++) {
        *(data + i) = i;
    }

    MemStatus status = my_memzero(data, size);
    assert_int_equal(status, MemStatus_Success);
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
    /**
       test reverse returns error for null pointer.
    */
    int length = 4;
    MemStatus status = my_reverse(NULL, length);
    assert_int_equal(status, MemStatus_Null_Pointer);
}

void test_my_reverse_even_length(void **state)
{
    /**
       test reverse returns reversed array for even length
    */
#define LENGTH 4 /**< @hideinitializer */
    int length = LENGTH;
    uint8_t data[LENGTH] = {0, 1, 2, 3};
    uint8_t expected[LENGTH] = {3, 2, 1, 0};
    MemStatus status = my_reverse(data, length);
    assert_int_equal(status, MemStatus_Success);
    assert_memory_equal(expected, data, length);
#undef LENGTH
}

void test_my_reverse_odd_length(void **state)
{
    /**
       test reverse returns reversed array for odd length
    */
#define LENGTH 5 /**< @hideinitializer */
    int length = LENGTH;
    uint8_t data[LENGTH] = {0, 1, 2, 3, 4};
    uint8_t expected[LENGTH] = {4, 3, 2, 1, 0};
    MemStatus status = my_reverse(data, length);
    assert_int_equal(status, MemStatus_Success);
    assert_memory_equal(expected, data, length);
#undef LENGTH
}

void test_my_reverse_zero_length(void **state)
{
    /**
       test reverse returns unmodified array for zero length
    */
#define LENGTH 5 /**< @hideinitializer */
    int length = LENGTH;
    uint8_t data[LENGTH] = {0, 1, 2, 3, 4};
    uint8_t expected[LENGTH] = {0, 1, 2, 3, 4};
    MemStatus status = my_reverse(data, 0);
    assert_int_equal(status, MemStatus_Success);
    assert_memory_equal(expected, data, length);
#undef LENGTH
}

void test_my_reverse_subarray_after(void **state)
{
    /**
       test reverse does not affect memory beyond specified length.
    */
#define LENGTH 5
    int length = LENGTH;
    uint8_t data[LENGTH] = {0, 1, 2, 3, 4};
    uint8_t expected[LENGTH] = {3, 2, 1, 0, 4};
    MemStatus status = my_reverse(data, 4);
    assert_int_equal(status, MemStatus_Success);
    assert_memory_equal(expected, data, length);
#undef LENGTH
}

void test_my_reverse_subarray_before(void **state)
{
    /**
       test reverse does not affect memory before the specified start
    */
#define LENGTH 5 /**< @hideinitializer */
    int length = LENGTH;
    uint8_t data[LENGTH] = {0, 1, 2, 3, 4};
    uint8_t expected[LENGTH] = {0, 4, 3, 2, 1};
    MemStatus status = my_reverse(data + 1, 4);
    assert_int_equal(status, MemStatus_Success);
    assert_memory_equal(expected, data, length);
#undef LENGTH
}

void test_my_reverse_all_characters(void **state)
{
    /**
       test reversing the entire array, even length
    */
    size_t const size = 256;
    uint8_t data[size];
    uint8_t expected[size];
    for (size_t i = 0; i < size; i++) {
        *(data + i) = i;
        *(expected + i) = size - 1 - i;
    }

    MemStatus status = my_reverse(data, size);
    assert_int_equal(status, MemStatus_Success);
    assert_memory_equal(expected, data, size);

}
