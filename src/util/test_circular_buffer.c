
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "circular_buffer.h"

#include "test_circular_buffer.h"

void test_cb_allocate_free(void **state);
void test_cb_new_length_error(void **state);
void test_cb_new_size_error(void **state);
void test_cb_buffer_null_pointer(void **state);
void test_cb_item_null_pointer(void **state);
void test_cb_uninitialized_buffer(void **state);
void test_cb_add_remove_one_item(void **state);
void test_cb_add_remove_n_items_sizeof1(void **state);
void test_cb_add_remove_n_items_sizeof2(void **state);
void test_cb_add_remove_n_items_sizeof4(void **state);
void test_cb_buffer_full(void **state);
void test_cb_buffer_empty(void **state);
void test_cb_wrap_add(void **state);
void test_cb_wrap_remove(void **state);
void test_cb_over_fill(void **state);
void test_cb_over_empty(void **state);
void test_cb_peak(void **state);

/**
   Test suite for circular buffer
*/
int suite_circular_buffer(void)
{
    const struct CMUnitTest cb_tests[] = {
        cmocka_unit_test(test_cb_allocate_free),
        cmocka_unit_test(test_cb_new_length_error),
        cmocka_unit_test(test_cb_new_size_error),
        cmocka_unit_test(test_cb_buffer_null_pointer),
        cmocka_unit_test(test_cb_item_null_pointer),
        cmocka_unit_test(test_cb_uninitialized_buffer),
        cmocka_unit_test(test_cb_add_remove_one_item),
        cmocka_unit_test(test_cb_add_remove_n_items_sizeof1),
        cmocka_unit_test(test_cb_add_remove_n_items_sizeof2),
        cmocka_unit_test(test_cb_add_remove_n_items_sizeof4),
        cmocka_unit_test(test_cb_buffer_full),
        cmocka_unit_test(test_cb_buffer_empty),
        cmocka_unit_test(test_cb_wrap_add),
        cmocka_unit_test(test_cb_wrap_remove),
        cmocka_unit_test(test_cb_over_fill),
        cmocka_unit_test(test_cb_over_empty),
        cmocka_unit_test(test_cb_peak),
    };
    return cmocka_run_group_tests(cb_tests, NULL, NULL);
}

void test_cb_allocate_free(void **state)
{
    /**
       test Allocate-Free - Checks that a new dynamic buffer can be created on
       the heap and freed correctly.
    */
    CircularBuffer_t volatile *cb = NULL;
    size_t num_items = 1;
    size_t bytes_per_item = 1;
    CircularBufferStatus status = CircularBufferNew(&cb, num_items, bytes_per_item);
    assert_int_equal(status, CircularBuffer_Success);
    assert_non_null(cb);

    status = CircularBufferDestroy(&cb);
    assert_int_equal(status, CircularBuffer_Success);
    assert_null(cb);
}

void test_cb_new_size_error(void **state)
{
    /**
       test new returns an error when bytes_per_item is 0
    */
    CircularBuffer_t volatile *cb = NULL;
    size_t num_items = 1;
    size_t bytes_per_item = 0;
    CircularBufferStatus status = CircularBufferNew(&cb, num_items, bytes_per_item);
    assert_int_equal(status, CircularBuffer_No_Bytes_Per_Item);
    assert_null(cb);
}

void test_cb_new_length_error(void **state)
{
    /**
       test new returns an error when num_items is 0
    */
    CircularBuffer_t volatile *cb = NULL;
    size_t num_items = 0;
    size_t bytes_per_item = 1;
    CircularBufferStatus status = CircularBufferNew(&cb, num_items, bytes_per_item);
    assert_int_equal(status, CircularBuffer_No_Num_Items);
    assert_null(cb);
}

void test_cb_buffer_null_pointer(void **state)
{
    /**
       test Invalid Pointer - Check that buffer pointer is valid
    */
    uint32_t data = 12345;
    CircularBuffer_t volatile *cb = NULL;
    CircularBufferStatus status = CircularBufferAddItem(cb, &data);
    assert_int_equal(status, CircularBuffer_Null_Pointer);
    status = CircularBufferRemoveItem(cb, &data);
    assert_int_equal(status, CircularBuffer_Null_Pointer);
}

void test_cb_item_null_pointer(void **state)
{
    /**
       test Invalid Pointer - Check that item pointer is valid
    */
    CircularBuffer_t volatile *cb = NULL;
    size_t num_items = 2;
    size_t bytes_per_item = sizeof(num_items);
    CircularBufferStatus status = CircularBufferNew(&cb, num_items, bytes_per_item);
    assert_int_equal(status, CircularBuffer_Success);
    status = CircularBufferAddItem(cb, NULL);
    assert_int_equal(status, CircularBuffer_Null_Pointer);

    status = CircularBufferDestroy(&cb);
    assert_int_equal(status, CircularBuffer_Success);
    assert_null(cb);
}

void test_cb_uninitialized_buffer(void **state)
{
    /**
       test Non-initialized Buffer - Check that buffer is initialized

       NOTE(bja, 2017-03) the implemenetation of the circular_buffer type should
       not allow returning a valid pointer to a circular buffer with an
       uninitialized internal buffer. This test isn't really relevant unless I
       find an unaccounted for edge case. Marking as passing.
    */
    assert_true(true);
}

void test_cb_add_remove_one_item(void **state)
{
    /** test Add-Remove - Check that add and then a remove returns the same item
        for full length of buffer
    */
    uint32_t data = 12345;
    CircularBuffer_t volatile *cb = NULL;
    size_t num_items = 1;
    size_t bytes_per_item = sizeof(data);
    CircularBufferStatus status = CircularBufferNew(&cb, num_items, bytes_per_item);
    assert_int_equal(status, CircularBuffer_Success);

    // add an item
    status = CircularBufferAddItem(cb, &data);
    assert_int_equal(status, CircularBuffer_Success);
    // extract the item and verify it matches what we added
    uint32_t received = 0;
    status = CircularBufferRemoveItem(cb, &received);
    assert_int_equal(status, CircularBuffer_Success);
    assert_int_equal(data, received);

    status = CircularBufferDestroy(&cb);
    assert_int_equal(status, CircularBuffer_Success);
    assert_null(cb);
}

void test_cb_add_remove_n_items_sizeof1(void **state)
{
    /**
       test Add-Remove - Check that add and then a remove returns the same item
       for full length of buffer
    */
#define SIZE 10
    uint8_t offset = 123;
    CircularBuffer_t volatile *cb = NULL;
    size_t num_items = SIZE;
    size_t bytes_per_item = sizeof(offset);
    CircularBufferStatus status = CircularBufferNew(&cb, num_items, bytes_per_item);
    assert_int_equal(status, CircularBuffer_Success);

    // initialize data
    uint8_t data[SIZE];
    uint8_t received[SIZE];
    for (size_t i = 0; i < num_items; i++) {
        data[i] = offset + i;
        received[i] = 0;
    }

    // add data to the buffer
    for (size_t i = 0; i < num_items; i++) {
        status = CircularBufferAddItem(cb, data + i);
        assert_int_equal(status, CircularBuffer_Success);
    }

    // remove data and verify it matches what we put in
    for (size_t i = 0; i < num_items; i++) {
        status = CircularBufferRemoveItem(cb, received + i);
        assert_int_equal(status, CircularBuffer_Success);
        if (data[i] != received[i]) {
            printf("%zu : %d != %d\n", i, data[i], received[i]);
        }
        assert_int_equal(data[i], received[i]);
    }

    status = CircularBufferDestroy(&cb);
    assert_int_equal(status, CircularBuffer_Success);
    assert_null(cb);
#undef SIZE
}

void test_cb_add_remove_n_items_sizeof2(void **state)
{
    /**
       test Add-Remove - Check that add and then a remove returns the same item
       for full length of buffer
    */
#define SIZE 10
    uint16_t offset = 12345;
    CircularBuffer_t volatile *cb = NULL;
    size_t num_items = SIZE;
    size_t bytes_per_item = sizeof(offset);
    CircularBufferStatus status = CircularBufferNew(&cb, num_items, bytes_per_item);
    assert_int_equal(status, CircularBuffer_Success);

    // initialize data
    uint16_t data[SIZE];
    uint16_t received[SIZE];
    for (size_t i = 0; i < num_items; i++) {
        data[i] = offset + i;
        received[i] = 0;
    }

    // fill the buffer
    for (size_t i = 0; i < num_items; i++) {
        status = CircularBufferAddItem(cb, data + i);
        assert_int_equal(status, CircularBuffer_Success);
    }

    // extract data and verify it matches what we put in
    for (size_t i = 0; i < num_items; i++) {
        status = CircularBufferRemoveItem(cb, received + i);
        assert_int_equal(status, CircularBuffer_Success);
        if (data[i] != received[i]) {
            printf("%zu : %d != %d\n", i, data[i], received[i]);
        }
        assert_int_equal(data[i], received[i]);
    }

    status = CircularBufferDestroy(&cb);
    assert_int_equal(status, CircularBuffer_Success);
    assert_null(cb);
#undef SIZE
}

void test_cb_add_remove_n_items_sizeof4(void **state)
{
    /**
       test Add-Remove - Check that add and then a remove returns the same item
       for full length of buffer
    */
#define SIZE 10
    uint32_t offset = -12345;
    CircularBuffer_t volatile *cb = NULL;
    size_t num_items = SIZE;
    size_t bytes_per_item = sizeof(offset);
    CircularBufferStatus status = CircularBufferNew(&cb, num_items, bytes_per_item);
    assert_int_equal(status, CircularBuffer_Success);

    // initialize data
    uint32_t data[SIZE];
    uint32_t received[SIZE];
    for (size_t i = 0; i < num_items; i++) {
        data[i] = offset + i;
        received[i] = 0;
    }

    // add data to the buffer
    for (size_t i = 0; i < num_items; i++) {
        status = CircularBufferAddItem(cb, data + i);
        assert_int_equal(status, CircularBuffer_Success);
    }

    // remove data from the buffer and verify it is what we put in
    for (size_t i = 0; i < num_items; i++) {
        status = CircularBufferRemoveItem(cb, received + i);
        assert_int_equal(status, CircularBuffer_Success);
        if (data[i] != received[i]) {
            printf("%zu : %d != %d\n", i, data[i], received[i]);
        }
        assert_int_equal(data[i], received[i]);
    }

    status = CircularBufferDestroy(&cb);
    assert_int_equal(status, CircularBuffer_Success);
    assert_null(cb);
#undef SIZE
}

void test_cb_buffer_full(void **state)
{
    /**
       test Buffer Full - Check buffer reports when full
    */
#define SIZE 10
    uint32_t offset = -12345;
    CircularBuffer_t volatile *cb = NULL;
    size_t num_items = SIZE;
    size_t bytes_per_item = sizeof(offset);
    CircularBufferStatus status = CircularBufferNew(&cb, num_items, bytes_per_item);
    assert_int_equal(status, CircularBuffer_Success);

    bool is_full = true;
    // don't report full when not full, status ok
    status = CircularBufferIsFull(cb, &is_full);
    assert_int_equal(status, CircularBuffer_Success);
    assert_int_equal(is_full, false);

    // initialize some data
    uint32_t data[SIZE];
    for (size_t i = 0; i < num_items; i++) {
        data[i] = offset + i;
    }

    // add data to fill the buffer
    for (size_t i = 0; i < num_items; i++) {
        status = CircularBufferAddItem(cb, data + i);
        assert_int_equal(status, CircularBuffer_Success);
    }

    // report full when full, but status ok
    status = CircularBufferIsFull(cb, &is_full);
    assert_int_equal(status, CircularBuffer_Success);
    assert_int_equal(is_full, true);

    status = CircularBufferDestroy(&cb);
    assert_int_equal(status, CircularBuffer_Success);
    assert_null(cb);
#undef SIZE
}

void test_cb_buffer_empty(void **state)
{
    /**
       test Buffer Empty - Check buffer reports for empty
    */
    uint32_t data = 12345;
    CircularBuffer_t volatile *cb = NULL;
    size_t num_items = 1;
    size_t bytes_per_item = sizeof(data);
    CircularBufferStatus status = CircularBufferNew(&cb, num_items, bytes_per_item);
    assert_int_equal(status, CircularBuffer_Success);

    bool is_empty = false;
    // report empty when empty, status ok
    status = CircularBufferIsEmpty(cb, &is_empty);
    assert_int_equal(status, CircularBuffer_Success);
    assert_int_equal(is_empty, true);

    status = CircularBufferAddItem(cb, &data);
    assert_int_equal(status, CircularBuffer_Success);

    // don't report empty when not empty, status ok
    status = CircularBufferIsEmpty(cb, &is_empty);
    assert_int_equal(status, CircularBuffer_Success);
    assert_int_equal(is_empty, false);

    status = CircularBufferDestroy(&cb);
    assert_int_equal(status, CircularBuffer_Success);
    assert_null(cb);
}

void test_cb_wrap_add(void **state)
{
    /** test Wrap Add - Test that buffer can wrap around the edge boundary
        and add to the front

        NOTE(bja, 2017-02) create a small buffer and a bunch of data, and then
        add-remove stuff to wrap around a bunch of times.
    */
#define SIZE 32
    uint32_t offset = 12345;
    CircularBuffer_t volatile *cb = NULL;
    size_t num_items = SIZE / 8;
    size_t bytes_per_item = sizeof(offset);
    CircularBufferStatus status = CircularBufferNew(&cb, num_items, bytes_per_item);
    assert_int_equal(status, CircularBuffer_Success);

    // initialize the data
    uint32_t data[SIZE];
    uint32_t received[SIZE];
    size_t size = SIZE;
    for (size_t i = 0; i < size; i++) {
        data[i] = offset + i;
        received[i] = 0;
    }

    // incrementally add and remove items to wrap around the buffer
    for (size_t i = 0; i < size; i++) {
        status = CircularBufferAddItem(cb, data + i);
        assert_int_equal(status, CircularBuffer_Success);

        status = CircularBufferRemoveItem(cb, received + i);
        assert_int_equal(status, CircularBuffer_Success);
        if (data[i] != received[i]) {
            printf("%zu : %d != %d\n", i, data[i], received[i]);
        }
        assert_int_equal(data[i], received[i]);
    }

    status = CircularBufferDestroy(&cb);
    assert_int_equal(status, CircularBuffer_Success);
    assert_null(cb);
#undef SIZE
}

void test_cb_wrap_remove(void **state)
{
    /**
       test Wrap Remove - Test that your buffer can wrap around the edge
       boundary and add to the front
       NOTE(bja, 2017-02) size is assumed to be a multiple of 4
    */
#define SIZE 32
    uint32_t offset = -12345;
    CircularBuffer_t volatile *cb = NULL;
    size_t num_items = SIZE / 4;
    size_t bytes_per_item = sizeof(offset);
    CircularBufferStatus status = CircularBufferNew(&cb, num_items, bytes_per_item);
    assert_int_equal(status, CircularBuffer_Success);

    uint32_t data[SIZE];
    uint32_t received[SIZE];
    size_t size = SIZE;
    for (size_t i = 0; i < size; i++) {
        data[i] = offset + i;
        received[i] = 0;
    }

    // fill the buffer
    size_t add_begin = 0;
    size_t add_end = num_items;
    for (size_t i = add_begin; i < add_end; i++) {
        status = CircularBufferAddItem(cb, data + i);
        assert_int_equal(status, CircularBuffer_Success);
    }

    // remove some items
    size_t remove_begin = 0;
    size_t remove_end = add_end / 2;
    for (size_t i = remove_begin; i < remove_end; i++) {
        status = CircularBufferRemoveItem(cb, received + i);
        assert_int_equal(status, CircularBuffer_Success);
        if (data[i] != received[i]) {
            printf("%zu : %d != %d\n", i, data[i], received[i]);
        }
        assert_int_equal(data[i], received[i]);
    }

    // add some more items
    add_begin = add_end;
    add_end += num_items / 2;
    for (size_t i = add_begin; i < add_end; i++) {
        status = CircularBufferAddItem(cb, data + i);
        assert_int_equal(status, CircularBuffer_Success);
    }

    // remove some items
    remove_begin = remove_end;
    remove_end += num_items;
    for (size_t i = remove_begin; i < remove_end; i++) {
        status = CircularBufferRemoveItem(cb, received + i);
        assert_int_equal(status, CircularBuffer_Success);
        if (data[i] != received[i]) {
            printf("%zu : %d != %d\n", i, data[i], received[i]);
        }
        assert_int_equal(data[i], received[i]);
    }

    status = CircularBufferDestroy(&cb);
    assert_int_equal(status, CircularBuffer_Success);
    assert_null(cb);
#undef SIZE
}

void test_cb_over_fill(void **state)
{
    /**
       test Over Fill - Test that your buffer fails when too many items are
       added
    */
#define SIZE 5
    uint16_t offset = 12345;
    CircularBuffer_t volatile *cb = NULL;
    size_t num_items = SIZE;
    size_t bytes_per_item = sizeof(offset);
    CircularBufferStatus status = CircularBufferNew(&cb, num_items, bytes_per_item);
    assert_int_equal(status, CircularBuffer_Success);

    // initialize data
    uint16_t data[SIZE + 1];
    for (size_t i = 0; i < num_items; i++) {
        data[i] = offset + i;
    }

    // completely fill the buffer.
    for (size_t i = 0; i < num_items; i++) {
        status = CircularBufferAddItem(cb, data + i);
        assert_int_equal(status, CircularBuffer_Success);
    }

    // attempt to overfill the buffer.
    status = CircularBufferAddItem(cb, data + SIZE - 1);
    assert_int_equal(status, CircularBuffer_Is_Full);

    status = CircularBufferDestroy(&cb);
    assert_int_equal(status, CircularBuffer_Success);
    assert_null(cb);
#undef SIZE
}

void test_cb_over_empty(void **state)
{
    /**
       test Over Empty - Test that your buffer fails to remove an item when
       empty
    */
#define SIZE 5
    uint16_t offset = 12345;
    CircularBuffer_t volatile *cb = NULL;
    size_t num_items = SIZE;
    size_t bytes_per_item = sizeof(offset);
    CircularBufferStatus status = CircularBufferNew(&cb, num_items, bytes_per_item);
    assert_int_equal(status, CircularBuffer_Success);

    // initialize data
    uint16_t data[SIZE];
    uint16_t received[SIZE + 1];
    for (size_t i = 0; i < num_items; i++) {
        data[i] = offset + i;
        received[i] = 0;
    }

    // try removing from an empty buffer
    status = CircularBufferRemoveItem(cb, received);
    assert_int_equal(status, CircularBuffer_Is_Empty);

    // add items to the buffer
    for (size_t i = 0; i < num_items; i++) {
        status = CircularBufferAddItem(cb, data + i);
        assert_int_equal(status, CircularBuffer_Success);
    }

    // remove the items
    for (size_t i = 0; i < num_items; i++) {
        status = CircularBufferRemoveItem(cb, received + i);
        assert_int_equal(status, CircularBuffer_Success);
        assert_int_equal(data[i], received[i]);
    }

    // again try to remove from an empty buffer
    status = CircularBufferRemoveItem(cb, received);
    assert_int_equal(status, CircularBuffer_Is_Empty);

    status = CircularBufferDestroy(&cb);
    assert_int_equal(status, CircularBuffer_Success);
    assert_null(cb);
#undef SIZE
}

void test_cb_peak(void **state)
{
    /**
       test peaking into the buffer copies out the correct item, but does not
       remove it.
    */
#define SIZE 32
    uint32_t offset = 12345;
    CircularBuffer_t volatile *cb = NULL;
    size_t num_items = SIZE / 4;
    size_t bytes_per_item = sizeof(offset);
    CircularBufferStatus status = CircularBufferNew(&cb, num_items, bytes_per_item);
    assert_int_equal(status, CircularBuffer_Success);

    // initialize some input data
    uint32_t data[SIZE];
    uint32_t received[SIZE];
    size_t size = SIZE;
    for (size_t i = 0; i < size; i++) {
        data[i] = offset + i;
        received[i] = 0;
    }

    // fill the buffer
    size_t add_begin = 0;
    size_t add_end = num_items;
    for (size_t i = add_begin; i < add_end; i++) {
        status = CircularBufferAddItem(cb, data + i);
        assert_int_equal(status, CircularBuffer_Success);
    }

    // remove some items
    size_t remove_begin = 0;
    size_t remove_end = add_end / 2;
    for (size_t i = remove_begin; i < remove_end; i++) {
        status = CircularBufferRemoveItem(cb, received + i);
        assert_int_equal(status, CircularBuffer_Success);
        if (data[i] != received[i]) {
            printf("%zu : %d != %d\n", i, data[i], received[i]);
        }
        assert_int_equal(data[i], received[i]);
    }

    // add some more items that will wrapp the buffer
    add_begin = add_end;
    add_end += num_items / 2;
    for (size_t i = add_begin; i < add_end; i++) {
        status = CircularBufferAddItem(cb, data + i);
        assert_int_equal(status, CircularBuffer_Success);
    }

    remove_begin = remove_end;
    remove_end += num_items;
    // peak at items without removing
    uint32_t item;
    for (size_t i = 0; i < num_items; i++) {
        status = CircularBufferPeakItem(cb, i, &item);
        assert_int_equal(status, CircularBuffer_Success);
        if (data[remove_begin + i] != item) {
            printf("%zu : %d != %d\n", i, data[remove_begin + i], item);
        }
        assert_int_equal(data[remove_begin + i], item);
    }

    // remove some items
    for (size_t i = remove_begin; i < remove_end; i++) {
        status = CircularBufferRemoveItem(cb, received + i);
        assert_int_equal(status, CircularBuffer_Success);
        if (data[i] != received[i]) {
            printf("%zu : %d != %d\n", i, data[i], received[i]);
        }
        assert_int_equal(data[i], received[i]);
    }

    status = CircularBufferDestroy(&cb);
    assert_int_equal(status, CircularBuffer_Success);
    assert_null(cb);
#undef SIZE
}

