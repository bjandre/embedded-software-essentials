#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "bit-operations.h"

#include "test_bit-operations.h"

void test_set_bit_true(void **state);
void test_set_bit_false(void **state);
void test_flip_bit_true(void **state);
void test_flip_bit_false(void **state);
void test_get_bit_true(void **state);
void test_get_bit_false(void **state);

int suite_bit_operations(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_set_bit_true),
        cmocka_unit_test(test_set_bit_false),
        cmocka_unit_test(test_flip_bit_true),
        cmocka_unit_test(test_flip_bit_false),
        cmocka_unit_test(test_get_bit_true),
        cmocka_unit_test(test_get_bit_false),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}

void test_set_bit_true(void **state)
{
    uint32_t field = 0b01010101010101010101010101010101;
    uint8_t shift = 5;
    uint32_t expected = 0b01010101010101010101010101110101;

    set_bit_one(&field,  shift);
    assert_int_equal(expected, field);
}

void test_set_bit_false(void **state)
{
    uint32_t field = 0b01010101010101010101010101010101;
    uint8_t shift = 24;
    uint32_t expected = 0b01010100010101010101010101010101;

    set_bit_zero(&field,  shift);
    assert_int_equal(expected, field);
}

void test_flip_bit_true(void **state)
{
    uint32_t field = 0b01010101010101010101010101010101;
    uint8_t shift = 15;
    uint32_t expected = 0b01010101010101011101010101010101;

    flip_bit(&field,  shift);
    assert_int_equal(expected, field);
}

void test_flip_bit_false(void **state)
{
    uint32_t field = 0b01010101010101010101010101010101;
    uint8_t shift = 28;
    uint32_t expected = 0b01000101010101010101010101010101;

    flip_bit(&field,  shift);
    assert_int_equal(expected, field);
}

void test_get_bit_true(void **state)
{
    uint32_t field = 0b00010000;
    uint8_t shift = 4;
    bool value;

    get_bit(&field,  shift, &value);
    assert_true(value);
}

void test_get_bit_false(void **state)
{
    uint32_t field = 0b11110111;
    uint8_t shift = 3;
    bool value;

    get_bit(&field,  shift, &value);
    assert_false(value);
}
