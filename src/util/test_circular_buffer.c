#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "circular_buffer.h"

#include "test_circular_buffer.h"

void test_cb_allocate_free(void **state);
void test_cb_null_pointer(void **state);
void test_cb_uninitialized_buffer(void **state);
void test_cb_add_remove(void **state);
void test_cb_buffer_full(void **state);
void test_cb_buffer_empty(void **state);
void test_cb_wrap_add(void **state);
void test_cb_wrap_remove(void **state);
void test_cb_over_fill(void **state);
void test_cb_over_empty(void **state);

int suite_circular_buffer(void)
{
    const struct CMUnitTest cb_tests[] = {
        cmocka_unit_test(test_cb_allocate_free),
        cmocka_unit_test(test_cb_null_pointer),
        cmocka_unit_test(test_cb_uninitialized_buffer),
        cmocka_unit_test(test_cb_add_remove),
        cmocka_unit_test(test_cb_buffer_full),
        cmocka_unit_test(test_cb_buffer_empty),
        cmocka_unit_test(test_cb_wrap_add),
        cmocka_unit_test(test_cb_wrap_remove),
        cmocka_unit_test(test_cb_over_fill),
        cmocka_unit_test(test_cb_over_empty),
    };
    return cmocka_run_group_tests(cb_tests, NULL, NULL);
}

void test_cb_allocate_free(void **state)
{
    // test Allocate-Free - Checks that a dynamic buffer can be created on the
    // heap
    skip();
}


void test_cb_null_pointer(void **state)
{
    // test Invalid Pointer - Check that buffer pointer is valid
    skip();
}

void test_cb_uninitialized_buffer(void **state)
{
    // test Non-initialized Buffer - Check that buffer is initialized
    skip();
}

void test_cb_add_remove(void **state)
{
    // test Add-Remove - Check that add and then a remove returns the same item
    // for full length of buffer
    skip();
}

void test_cb_buffer_full(void **state)
{
    // test Buffer Full - Check buffer reports true for full
    skip();
}

void test_cb_buffer_empty(void **state)
{
    // test Buffer Empty - Check buffer reports true for full
    skip();
}

void test_cb_wrap_add(void **state)
{
    // test Wrap Add - Test that your buffer can wrap around the edge boundary
    // and add to the front
    skip();
}

void test_cb_wrap_remove(void **state)
{
    // test Wrap Remove - Test that your buffer can wrap around the edge
    // boundary and add to the front
    skip();
}

void test_cb_over_fill(void **state)
{
    // test Over Fill - Test that your buffer fails when too many items are
    // added
    skip();
}

void test_cb_over_empty(void **state)
{
    // test Over Empty - Test that your buffer fails to remove an item when
    // empty
    skip();
}

