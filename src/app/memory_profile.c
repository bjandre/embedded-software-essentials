
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

#include "async_global.h"
#include "log_item.h"
#include "profiling_timer_data.h"
#include "profiling_timer.h"
#include "profiler.h"
#include "memory_dma.h"
#include "memory_cpu.h"
#include "memory_profile.h"

const uint8_t num_trials = 100;

/**
 */
void profile_memmove(log_item_t *item, uint32_t num_bytes, uint8_t *destination,
                     uint8_t *source);

/**

 */
void profile_memset(log_item_t *item, uint32_t num_bytes, uint8_t *destination,
                    int32_t *source);


void memory_profile(log_item_t *item)
{
    enable_global_async_profiling_active();
    uint32_t cps = get_clocks_per_second_profiling_timer();

    log_item_update(item, PROFILING_CLOCKS_PER_SEC, sizeof(cps), &(cps));
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
    uint8_t *destination = (uint8_t *)malloc(bytes[num_bytes - 1]);
    uint8_t *source = (uint8_t *)malloc(bytes[num_bytes - 1]);
    int32_t set_value = 0xAAAAAAAA;

    const char name_total_bytes[] = "total_bytes";
    for (size_t nb = 0; nb < num_bytes; nb++) {
        log_item_update(item, PROFILING_START, sizeof(name_total_bytes),
                        &name_total_bytes);
        log_item(item);
        log_item_update(item, PROFILING_VALUE, sizeof(uint16_t), bytes + nb);
        log_item(item);

        profile_memset(item, bytes[nb], destination, &set_value);
        profile_memmove(item, bytes[nb], destination, source);

        log_item_update(item, PROFILING_END, sizeof(name_total_bytes),
                        &name_total_bytes);
        log_item(item);
    }

    free(destination);
    free(source);

    enable_global_async_profiling_active();
}

void profile_memmov_dma(uint32_t num_bytes,
                        log_item_t *item, uint8_t *destination, uint8_t *source)
{
#define NUM_BYTES_PER_TRANSFER 3
    const uint32_t num_bytes_per_transfer = NUM_BYTES_PER_TRANSFER;
    uint32_t bytes_per_transfer[NUM_BYTES_PER_TRANSFER] = { 1, 2, 4 };
#undef NUM_BYTES_PER_TRANSFER
    profiling_timer_data_t start_time, end_time;
    uint32_t total_clocks;
    uint32_t average_clocks;

    const char name_memmove_dma[] = "memmove_dma";
    log_item_update(item, PROFILING_START, sizeof(name_memmove_dma),
                    &name_memmove_dma);
    log_item(item);
    for (size_t nbpt = 0; nbpt < num_bytes_per_transfer; nbpt++) {
        total_clocks = 0;
        char str_nbpt = '0' + bytes_per_transfer[nbpt];
        log_item_update(item, PROFILING_START, sizeof(char), &str_nbpt);
        log_item(item);
        for (uint8_t i = 0; i < num_trials; i++) {
            bool dma_complete = false;
            get_timer(&start_time);
            memmove_dma(destination, source, num_bytes,
                        bytes_per_transfer[nbpt]);
            /* wait for dma to finish and set end time in interrupt! */
            while (!dma_complete) {
                dma_complete = get_global_async_dma_complete();
            }
            end_time = get_global_async_profiling_end_time();
            total_clocks += elapsed_time(&start_time, &end_time);
        }
        average_clocks = total_clocks / num_trials;
        log_item_update(item, PROFILING_VALUE, sizeof(uint32_t), &average_clocks);
        log_item(item);
        log_item_update(item, PROFILING_END, sizeof(char), &str_nbpt);
        log_item(item);
    }
    log_item_update(item, PROFILING_END, sizeof(name_memmove_dma),
                    &name_memmove_dma);
    log_item(item);
}

typedef void *(*memmove_fixed_transfer_size)(void *destination,
        const void *source, size_t num );

void profile_memmove_no_transfer_size(log_item_t *item,
                                      memmove_fixed_transfer_size memfunc,
                                      char *name, size_t size_name,
                                      uint32_t num_bytes, uint8_t *destination, uint8_t *source)
{
    profiling_timer_data_t start_time, end_time;
    uint32_t total_clocks;
    uint32_t average_clocks;

    log_item_update(item, PROFILING_START, size_name, name);
    log_item(item);

    total_clocks = 0;
    for (uint8_t i = 0; i < num_trials; i++) {
        get_timer(&start_time);
        memfunc(destination, source, num_bytes);
        get_timer(&end_time);
        total_clocks += elapsed_time(&start_time, &end_time);
    }
    average_clocks = total_clocks / num_trials;
    log_item_update(item, PROFILING_VALUE, sizeof(uint32_t), &average_clocks);
    log_item(item);

    log_item_update(item, PROFILING_END, size_name, name);
    log_item(item);
}

#define PROFILE_MEMMOVE(item, memfunc, num_bytes, destination, source)  \
    profile_memmove_no_transfer_size(item, memfunc, #memfunc, sizeof(#memfunc), num_bytes, destination, source)

void profile_memmove(log_item_t *item, uint32_t num_bytes, uint8_t *destination,
                     uint8_t *source)
{
    memmove_fixed_transfer_size stdlib_memmove = (memmove_fixed_transfer_size)(
                &memmove);
    PROFILE_MEMMOVE(item, stdlib_memmove, num_bytes, destination, source);

    memmove_fixed_transfer_size cpu_memmove = (memmove_fixed_transfer_size)(
                &memmove_cpu);
    PROFILE_MEMMOVE(item, cpu_memmove, num_bytes, destination, source);

    profile_memmov_dma(num_bytes, item, destination, source);
}


void profile_memset_dma(uint32_t num_bytes,
                        log_item_t *item, uint8_t *destination, uint8_t *source)
{
#define NUM_BYTES_PER_TRANSFER 3
    const uint32_t num_bytes_per_transfer = NUM_BYTES_PER_TRANSFER;
    uint32_t bytes_per_transfer[NUM_BYTES_PER_TRANSFER] = { 1, 2, 4 };
#undef NUM_BYTES_PER_TRANSFER
    profiling_timer_data_t start_time, end_time;
    uint32_t total_clocks;
    uint32_t average_clocks;

    const char name_memset_dma[] = "memset_dma";
    log_item_update(item, PROFILING_START, sizeof(name_memset_dma),
                    &name_memset_dma);
    log_item(item);
    for (size_t nbpt = 0; nbpt < num_bytes_per_transfer; nbpt++) {
        total_clocks = 0;
        char str_nbpt = '0' + bytes_per_transfer[nbpt];
        log_item_update(item, PROFILING_START, sizeof(char), &str_nbpt);
        log_item(item);
        for (uint8_t i = 0; i < num_trials; i++) {
            bool dma_complete = false;
            get_timer(&start_time);
            memset_dma(destination, source, num_bytes,
                       bytes_per_transfer[nbpt]);
            /* wait for dma to finish and set end time in interrupt! */
            while (!dma_complete) {
                dma_complete = get_global_async_dma_complete();
            }
            end_time = get_global_async_profiling_end_time();
            total_clocks += elapsed_time(&start_time, &end_time);
        }
        average_clocks = total_clocks / num_trials;
        log_item_update(item, PROFILING_VALUE, sizeof(uint32_t), &average_clocks);
        log_item(item);
        log_item_update(item, PROFILING_END, sizeof(char), &str_nbpt);
        log_item(item);
    }
    log_item_update(item, PROFILING_END, sizeof(name_memset_dma),
                    &name_memset_dma);
    log_item(item);
}

typedef void *(*memset_fixed_transfer_size)(void *destination, int32_t source,
        size_t num );

void profile_memset_no_transfer_size(log_item_t *item,
                                     memset_fixed_transfer_size memfunc,
                                     char *name, size_t size_name,
                                     uint32_t num_bytes, uint8_t *destination, int8_t source)
{
    profiling_timer_data_t start_time, end_time;
    uint32_t total_clocks = 0;
    uint32_t average_clocks = 0;
    total_clocks = 0;
    log_item_update(item, PROFILING_START, size_name, name);
    log_item(item);
    for (uint8_t i = 0; i < num_trials; i++) {
        get_timer(&start_time);
        memset(destination, source, num_bytes);
        get_timer(&end_time);
        total_clocks += elapsed_time(&start_time, &end_time);
    }
    average_clocks = total_clocks / num_trials;
    log_item_update(item, PROFILING_VALUE, sizeof(uint32_t), &average_clocks);
    log_item(item);
    log_item_update(item, PROFILING_END, size_name, name);
    log_item(item);
}


#define PROFILE_MEMSET(item, memfunc, num_bytes, destination, source) \
    profile_memset_no_transfer_size(item, memfunc, #memfunc, sizeof(#memfunc), num_bytes, destination, source)

void profile_memset(log_item_t *item, uint32_t num_bytes, uint8_t *destination,
                    int32_t *source)
{
    memset_fixed_transfer_size stdlib_memset = (memset_fixed_transfer_size)(
                &memset);
    PROFILE_MEMSET(item, stdlib_memset, num_bytes, destination, *source);

    memset_fixed_transfer_size cpu_memset = (memset_fixed_transfer_size)(
            &memset_cpu);
    PROFILE_MEMSET(item, cpu_memset, num_bytes, destination, *((int8_t *)source));
    /* NOTE: dma can operate on 1, 2 , or 4 bytes */
    profile_memset_dma(num_bytes, item, destination, (uint8_t *)source);
}
