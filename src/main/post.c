
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include <stdint.h>

#include "platform-defs.h"
#include "post-common.h"
#include "post.h"

#include "post-profiler.h"

#if (PLATFORM == PLATFORM_FRDM)
#include "post-dma-frdm-kl25z.h"
#endif

typedef struct _suite_status {
    size_t num_tests_run;
    size_t num_tests_pass;
} suite_status_t;

typedef POSTstatus (*post_test)(void);

void run_test(log_item_t *item, post_test test, const char *name,
              size_t name_size, suite_status_t *suite_status);

void run_test(log_item_t *item, post_test test, const char *name,
              size_t name_size, suite_status_t *suite_status)
{
    suite_status->num_tests_run++;
    POSTstatus status = (*test)();
    if (POST_PASS != status) {
        UpdateLogItem(item, POST_ERROR, name_size, (uint8_t *)name);
        log_item(item);
    } else {
        suite_status->num_tests_pass++;
    }
}

void power_on_self_tests(log_item_t *item)
{
    UpdateLogItemNoPayload(item, POST_START);
    log_item(item);

    suite_status_t suite_status;
    suite_status.num_tests_run = 0;
    suite_status.num_tests_pass = 0;

    {
        const char name[] = "post_profiler_nop";
        run_test(item, &post_profiler_nop, name, sizeof(name), &suite_status);
    }

#if (PLATFORM == PLATFORM_FRDM)
    {
        const char name[] = "post_dma_memmove_1byte";
        run_test(item, &post_dma_memmove_1byte, name, sizeof(name), &suite_status);
    }
    {
        const char name[] = "post_dma_memmove_4byte";
        run_test(item, &post_dma_memmove_4byte, name, sizeof(name), &suite_status);
    }
    {
        const char name[] = "post_dma_memset_1byte";
        run_test(item, &post_dma_memset_1byte, name, sizeof(name), &suite_status);
    }
    {
        const char name[] = "post_dma_memset_4byte";
        run_test(item, &post_dma_memset_4byte, name, sizeof(name), &suite_status);
    }
#endif

    UpdateLogItem(item, POST_COMPLETE, sizeof(size_t),
                  &suite_status.num_tests_pass);
    log_item(item);
    // FIXME(bja, 2017-04) log num tests passed and num tests run!
}
