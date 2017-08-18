
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "math_util.h"

#include "test_math_util.h"

void test_next_pow_2_zero(void **state);
void test_next_pow_2_one(void **state);
void test_next_pow_2_two(void **state);
void test_next_pow_2_three(void **state);
void test_next_pow_2_fifteen(void **state);
void test_next_pow_2_seventeen(void **state);

/**
   Test suite for bit operations utilities.
*/
int suite_math_util(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_next_pow_2_zero),
        cmocka_unit_test(test_next_pow_2_one),
        cmocka_unit_test(test_next_pow_2_two),
        cmocka_unit_test(test_next_pow_2_three),
        cmocka_unit_test(test_next_pow_2_fifteen),
        cmocka_unit_test(test_next_pow_2_seventeen),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}

void test_next_pow_2_zero(void **state)
{
    uint32_t value = 0;
    uint32_t expected = 1;

    uint32_t received = next_power_of_2(value);
    assert_int_equal(expected, received);
}

void test_next_pow_2_one(void **state)
{
    uint32_t value = 1;
    uint32_t expected = 1;

    uint32_t received = next_power_of_2(value);
    assert_int_equal(expected, received);
}

void test_next_pow_2_two(void **state)
{
    uint32_t value = 2;
    uint32_t expected = 2;

    uint32_t received = next_power_of_2(value);
    assert_int_equal(expected, received);
}

void test_next_pow_2_three(void **state)
{
    uint32_t value = 3;
    uint32_t expected = 4;

    uint32_t received = next_power_of_2(value);
    assert_int_equal(expected, received);
}

void test_next_pow_2_fifteen(void **state)
{
    uint32_t value = 15;
    uint32_t expected = 16;

    uint32_t received = next_power_of_2(value);
    assert_int_equal(expected, received);
}

void test_next_pow_2_seventeen(void **state)
{
    uint32_t value = 17;
    uint32_t expected = 32;

    uint32_t received = next_power_of_2(value);
    assert_int_equal(expected, received);
}

