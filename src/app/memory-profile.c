
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "async-global.h"
#include "log-item.h"
#include "profiling-timer-data.h"
#include "profiler.h"
#include "memory-dma.h"
#include "memory-cpu.h"
#include "memory-profile.h"

const uint8_t num_trials = 100;

void profile_memmove(log_item_t *item, uint32_t num_bytes);
void profile_memset(log_item_t *item, uint32_t num_bytes);

void memory_profile(log_item_t *item)
{
    enable_global_async_profiling_active();
    /* 10 Bytes
       100 Bytes
       1000 Bytes
       5000 Bytes
    */
#define NUM_LENGTHS 4
    const uint32_t num_lengths = NUM_LENGTHS;
    uint32_t lengths[NUM_LENGTHS] = {10, 100, 1000, 5000};
#undef NUM_LENGTHS

    for (int i = 0; i < num_lengths; i++) {
        profile_memmove(item, lengths[i]);
        profile_memset(item, lengths[i]);
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
    average_clocks = 0;

    const char name_memmove_stdlib[] = "memmove_stdlib";
    UpdateLogItem(item, PROFILING_START, sizeof(name_memmove_stdlib), &name_memmove_stdlib);
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
    UpdateLogItem(item, PROFILING_END, sizeof(name_memmove_stdlib), &name_memmove_stdlib);
    log_item(item);

    const char name_memmove_cpu[] = "memmove_cpu";
    UpdateLogItem(item, PROFILING_START, sizeof(name_memmove_cpu), &name_memmove_cpu);
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

    const char name_memmove_dma[] = "memmove_dma";
    UpdateLogItem(item, PROFILING_START, sizeof(name_memmove_dma), &name_memmove_dma);
    log_item(item);
    for (uint8_t i = 0; i < num_trials; i++) {
        bool dma_complete = false;
        get_timer(&start_time);
        memmove_dma(destination, source, num_bytes, sizeof(uint8_t));
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
    uint32_t total_clocks;
    uint32_t average_clocks;

    total_clocks = 0;
    average_clocks = 0;

    const char name_memset_stdlib[] = "memset_stdlib";
    UpdateLogItem(item, PROFILING_START, sizeof(name_memset_stdlib), &name_memset_stdlib);
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
    UpdateLogItem(item, PROFILING_END, sizeof(name_memset_stdlib), &name_memset_stdlib);
    log_item(item);

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
