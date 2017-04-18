
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

#include "profiling-timer-data.h"
#include "profiler.h"

#include "test_profiler.h"

#include "async-global.h"

volatile async_data_t global_async_data;

void test_profiler_elapsed_time_zero_overflow(void **state);
void test_profiler_elapsed_time_one_overflow(void **state);
void test_profiler_elapsed_time_two_overflow(void **state);

/**
   Test suite for bit operations utilities.
*/
int suite_profiler(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_profiler_elapsed_time_zero_overflow),
        cmocka_unit_test(test_profiler_elapsed_time_one_overflow),
        cmocka_unit_test(test_profiler_elapsed_time_two_overflow),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}

void test_profiler_elapsed_time_zero_overflow(void **state)
{
    profiling_timer_data_t start_time;
    start_time.timer_count = 100;
    start_time.overflow_count = 1;
    start_time.max_timer_value = 0xFF;
    profiling_timer_data_t end_time;
    end_time.timer_count = 200;
    end_time.overflow_count = 1;
    end_time.max_timer_value = 0xFF;

    uint32_t expected = 100;

    uint32_t received = elapsed_time(&start_time, &end_time);
    assert_int_equal(expected, received);
}


void test_profiler_elapsed_time_one_overflow(void **state)
{
    profiling_timer_data_t start_time;
    start_time.timer_count = 200;
    start_time.overflow_count = 1;
    start_time.max_timer_value = 0xFF;
    profiling_timer_data_t end_time;
    end_time.timer_count = 45;
    end_time.overflow_count = 2;
    end_time.max_timer_value = 0xFF;

    uint32_t expected = 100;

    uint32_t received = elapsed_time(&start_time, &end_time);
    assert_int_equal(expected, received);
}

void test_profiler_elapsed_time_two_overflow(void **state)
{
    profiling_timer_data_t start_time;
    start_time.timer_count = 200;
    start_time.overflow_count = 1;
    start_time.max_timer_value = 0xFF;
    profiling_timer_data_t end_time;
    end_time.timer_count = 45;
    end_time.overflow_count = 3;
    end_time.max_timer_value = 0xFF;

    uint32_t expected = 355;

    uint32_t received = elapsed_time(&start_time, &end_time);
    assert_int_equal(expected, received);
}


