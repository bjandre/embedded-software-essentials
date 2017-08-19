
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include <stdint.h>

#include "log_item.h"
#include "leds.h"

#include "post_common.h"
#include "post.h"

#include "post_profiler.h"
#include "post_dma.h"
#include "post_nrf24l01.h"


typedef struct _suite_status {
    size_t num_tests_run;
    size_t num_tests_pass;
    size_t num_tests_skipped;
} suite_status_t;

#define POST_TEST(item,test,status) run_test(item, &test, #test, sizeof(#test), &status)

typedef POSTstatus (*post_test)(void);

void run_test(log_item_t *item, post_test test, const char *name,
              size_t name_size, suite_status_t *suite_status);

void run_test(log_item_t *item, post_test test, const char *name,
              size_t name_size, suite_status_t *suite_status)
{
    suite_status->num_tests_run++;
    POSTstatus status = (*test)();
    if (POST_SKIPPED == status) {
        suite_status->num_tests_skipped++;
    } else if (POST_PASS == status) {
        suite_status->num_tests_pass++;
    } else {/* (POST_PASS != status) */
        log_item_update(item, POST_ERROR, name_size, (uint8_t *)name);
        log_item(item);
    }
}

void power_on_self_tests(log_item_t *item)
{
    leds_on_off(LEDS_GREEN);

    log_item_update_no_payload(item, POST_START);
    log_item(item);

    suite_status_t suite_status;
    suite_status.num_tests_run = 0;
    suite_status.num_tests_pass = 0;
    suite_status.num_tests_skipped = 0;

    POST_TEST(item, post_profiler_nop, suite_status);

    POST_TEST(item, post_dma_memmove_1byte, suite_status);
    POST_TEST(item, post_dma_memmove_4byte, suite_status);
    POST_TEST(item, post_dma_memset_1byte, suite_status);
    POST_TEST(item, post_dma_memset_4byte, suite_status);

    POST_TEST(item, post_nrf24_read_status, suite_status);
    POST_TEST(item, post_nrf24_read_write_config, suite_status);
    POST_TEST(item, post_nrf24_read_write_rf_setup, suite_status);
    POST_TEST(item, post_nrf24_read_write_rf_channel, suite_status);
    POST_TEST(item, post_nrf24_read_write_tx_addr, suite_status);
    POST_TEST(item, post_nrf24_read_fifo_status, suite_status);
    POST_TEST(item, post_nrf24_flush_tx_fifo, suite_status);
    POST_TEST(item, post_nrf24_flush_rx_fifo, suite_status);

    log_item_update(item, POST_NUM_TESTS_RUN, sizeof(size_t),
                    &suite_status.num_tests_run);
    log_item(item);

    log_item_update(item, POST_NUM_TESTS_SKIPPED, sizeof(size_t),
                    &suite_status.num_tests_skipped);
    log_item(item);

    log_item_update(item, POST_NUM_TESTS_PASSED, sizeof(size_t),
                    &suite_status.num_tests_pass);
    log_item(item);

    log_item_update_no_payload(item, POST_COMPLETE);
    log_item(item);

    leds_on_off(LEDS_GREEN);
}
