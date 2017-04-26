
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include <stdint.h>

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

//#include "test_template.h"
#include "template.h"

void test_template_func(void **state);

/**
   Test suite for template
*/
int suite_data(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_template_func),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}

void test_template_func(void **state)
{
    /**
       test for template_func
    */
    uint32_t const expected = 0;

    uint32_t status = template_func(NULL);
    assert_int_equal(status, expected);
}
