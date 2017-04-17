
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "async-global.h"
#include "log-item.h"
#include "profiling-timer-data.h"
#include "profiling-timer.h"
#include "profiler.h"
#include "memory-dma.h"
#include "memory-cpu.h"
#include "memory-profile.h"

const uint8_t num_trials = 100;

/**
 */
void profile_memmove(log_item_t *item, uint32_t num_bytes);

/**

 */
void profile_memset(log_item_t *item, uint32_t num_bytes);

void memory_profile(log_item_t *item)
{
    enable_global_async_profiling_active();
    uint32_t cps = get_clocks_per_second_profiling_timer();

    UpdateLogItem(item, PROFILING_CLOCKS_PER_SEC, sizeof(cps), &(cps));
    log_item(item);
    /* NOTE(bja, 2017-04 ) replacing the requested number of bytes from the
     * assignment with multiples of 4 bytes to ensure transfer are evenly
     * divisible by transfer sizes and avoid a bunch of pointless one-off cpu copies
     * that don't contribute to profiling. Keeping multiplication factor between sizes

       10 Bytes -> 12 bytes
       100 Bytes
       1000 Bytes
       5000 Bytes
    */

#define NUM_BYTES 4
    const size_t num_bytes = NUM_BYTES;
    uint16_t bytes[NUM_BYTES] = {12u, 100u, 1000u, 5000u};
#undef NUM_BYTES

    const char name_total_bytes[] = "total_bytes";
    for (size_t nb = 0; nb < num_bytes; nb++) {
        UpdateLogItem(item, PROFILING_START, sizeof(name_total_bytes),
                      &name_total_bytes);
        log_item(item);
        UpdateLogItem(item, PROFILING_VALUE, sizeof(uint16_t), bytes + nb);
        log_item(item);

        profile_memset(item, bytes[nb]);
        profile_memmove(item, bytes[nb]);

        UpdateLogItem(item, PROFILING_END, sizeof(name_total_bytes), &name_total_bytes);
        log_item(item);
    }

    enable_global_async_profiling_active();
}


void profile_memmove(log_item_t *item, uint32_t num_bytes)
{
    uint8_t *destination = malloc(num_bytes);
    uint8_t *source = malloc(num_bytes);
    profiling_timer_data_t start_time, end_time;
    uint32_t total_clocks;
    uint32_t average_clocks;

    total_clocks = 0;

    const char name_memmove_stdlib[] = "memmove_stdlib";
    UpdateLogItem(item, PROFILING_START, sizeof(name_memmove_stdlib),
                  &name_memmove_stdlib);
    log_item(item);
    for (uint8_t i = 0; i < num_trials; i++) {
        get_timer(&start_time);
        memmove(destination, source, num_bytes);
        get_timer(&end_time);
        total_clocks += elapsed_time(&start_time, &end_time);
    }
    average_clocks = total_clocks / num_trials;
    UpdateLogItem(item, PROFILING_VALUE, sizeof(uint32_t), &average_clocks);
    log_item(item);
    UpdateLogItem(item, PROFILING_END, sizeof(name_memmove_stdlib),
                  &name_memmove_stdlib);
    log_item(item);

    total_clocks = 0;

    const char name_memmove_cpu[] = "memmove_cpu";
    UpdateLogItem(item, PROFILING_START, sizeof(name_memmove_cpu),
                  &name_memmove_cpu);
    log_item(item);
    for (uint8_t i = 0; i < num_trials; i++) {
        get_timer(&start_time);
        memmove_cpu(destination, source, num_bytes);
        get_timer(&end_time);
        total_clocks += elapsed_time(&start_time, &end_time);
    }
    average_clocks = total_clocks / num_trials;
    UpdateLogItem(item, PROFILING_VALUE, sizeof(uint32_t), &average_clocks);
    log_item(item);
    UpdateLogItem(item, PROFILING_END, sizeof(name_memmove_cpu), &name_memmove_cpu);
    log_item(item);

#define NUM_BYTES_PER_TRANSFER 3
    const uint32_t num_bytes_per_transfer = NUM_BYTES_PER_TRANSFER;
    uint32_t bytes_per_transfer[NUM_BYTES_PER_TRANSFER] = {1, 2, 4};
#undef NUM_BYTES_PER_TRANSFER

    const char name_memmove_dma[] = "memmove_dma";
    UpdateLogItem(item, PROFILING_START, sizeof(name_memmove_dma),
                  &name_memmove_dma);
    log_item(item);
    for (size_t nbpt = 0; nbpt < num_bytes_per_transfer; nbpt++) {
        total_clocks = 0;
        char str_nbpt = '0' + bytes_per_transfer[nbpt];
        UpdateLogItem(item, PROFILING_START, sizeof(char), &str_nbpt);
        log_item(item);
        for (uint8_t i = 0; i < num_trials; i++) {
            bool dma_complete = false;
            get_timer(&start_time);
            memmove_dma(destination, source, num_bytes, bytes_per_transfer[nbpt]);
            // wait for dma to finish and set end time in interrupt!
            while (!dma_complete) {
                dma_complete = get_global_async_dma_complete();
            }
            end_time = get_global_async_profiling_end_time();
            total_clocks += elapsed_time(&start_time, &end_time);
        }
        average_clocks = total_clocks / num_trials;
        UpdateLogItem(item, PROFILING_VALUE, sizeof(uint32_t), &average_clocks);
        log_item(item);
        UpdateLogItem(item, PROFILING_END, sizeof(char), &str_nbpt);
        log_item(item);
    }
    UpdateLogItem(item, PROFILING_END, sizeof(name_memmove_dma), &name_memmove_dma);
    log_item(item);

    free(destination);
    free(source);
}

void profile_memset(log_item_t *item, uint32_t num_bytes)
{
    uint8_t *destination = malloc(num_bytes);
    uint8_t source = 0xAA;
    profiling_timer_data_t start_time, end_time;
    uint32_t total_clocks = 0;
    uint32_t average_clocks = 0;

    total_clocks = 0;

    const char name_memset_stdlib[] = "memset_stdlib";
    UpdateLogItem(item, PROFILING_START, sizeof(name_memset_stdlib),
                  &name_memset_stdlib);
    log_item(item);
    for (uint8_t i = 0; i < num_trials; i++) {
        get_timer(&start_time);
        memset(destination, source, num_bytes);
        get_timer(&end_time);
        total_clocks += elapsed_time(&start_time, &end_time);
    }
    average_clocks = total_clocks / num_trials;
    UpdateLogItem(item, PROFILING_VALUE, sizeof(uint32_t), &average_clocks);
    log_item(item);
    UpdateLogItem(item, PROFILING_END, sizeof(name_memset_stdlib),
                  &name_memset_stdlib);
    log_item(item);

    total_clocks = 0;

    const char name_memset_cpu[] = "memset_cpu";
    UpdateLogItem(item, PROFILING_START, sizeof(name_memset_cpu), &name_memset_cpu);
    log_item(item);
    for (uint8_t i = 0; i < num_trials; i++) {
        get_timer(&start_time);
        memset_cpu(destination, &source, num_bytes);
        get_timer(&end_time);
        total_clocks += elapsed_time(&start_time, &end_time);
    }
    average_clocks = total_clocks / num_trials;
    UpdateLogItem(item, PROFILING_VALUE, sizeof(uint32_t), &average_clocks);
    log_item(item);
    UpdateLogItem(item, PROFILING_END, sizeof(name_memset_cpu), &name_memset_cpu);
    log_item(item);

    /**
       NOTE(bja, 2017-04) by definition in the standard library, memset operates on
       single bytes. Therefore, it doesn't make sense or provide a fair comparison to
       profile multi-byte DMA transfer for memset.
     */
    total_clocks = 0;

    const char name_memset_dma[] = "memset_dma";
    UpdateLogItem(item, PROFILING_START, sizeof(name_memset_dma), &name_memset_dma);
    log_item(item);
    for (uint8_t i = 0; i < num_trials; i++) {
        bool dma_complete = false;
        get_timer(&start_time);
        memset_dma(destination, &source, num_bytes, sizeof(uint8_t));
        // wait for dma to finish and set end time in interrupt!
        while (!dma_complete) {
            dma_complete = get_global_async_dma_complete();
        }
        end_time = get_global_async_profiling_end_time();
        total_clocks += elapsed_time(&start_time, &end_time);
    }
    average_clocks = total_clocks / num_trials;
    UpdateLogItem(item, PROFILING_VALUE, sizeof(uint32_t), &average_clocks);
    log_item(item);
    UpdateLogItem(item, PROFILING_END, sizeof(name_memset_dma), &name_memset_dma);
    log_item(item);

    free(destination);
}
