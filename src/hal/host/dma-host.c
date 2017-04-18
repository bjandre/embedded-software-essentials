
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
#include <stdlib.h>
#include <string.h>

#include "memory-common.h"
#include "dma-host.h"
#include "profiler.h"
#include "profiling-timer-data.h"
#include "async-global.h"

void dummy_DMA2_IRQHandler(void);

void host_initialize_dma(void)
{
    /* Nothing to do. */
}


MemStatus host_memmove_dma(uint8_t *destination, uint8_t const *const source,
                           uint32_t const num_bytes, uint8_t const bytes_per_transfer)
{
    if (NULL == destination || NULL == source) {
        return MemStatus_Null_Pointer;
    }

    if (bytes_per_transfer != 1 &&
            bytes_per_transfer != 2 &&
            bytes_per_transfer != 4) {
        return MemStatus_Transfer_Size_Error;
    }

    memmove(destination, source, num_bytes);
    dummy_DMA2_IRQHandler();

    return MemStatus_Success;
}

MemStatus host_memset_dma(uint8_t *destination, uint8_t const *const source,
                          uint32_t const num_bytes, uint8_t const bytes_per_transfer)
{
    if (NULL == destination || NULL == source) {
        return MemStatus_Null_Pointer;
    }

    if (bytes_per_transfer != 1 &&
            bytes_per_transfer != 2 &&
            bytes_per_transfer != 4) {
        return MemStatus_Transfer_Size_Error;
    }

    memset(destination, *source, num_bytes);
    dummy_DMA2_IRQHandler();

    return MemStatus_Success;
}

void dummy_DMA2_IRQHandler(void)
{
    set_global_async_dma_complete(true);
    bool profiling_active = get_global_async_profiling_active();
    if (profiling_active) {
        profiling_timer_data_t end_time;
        get_timer(&end_time);
        set_global_async_profiling_end_time(end_time);
    }
}
