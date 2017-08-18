
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/
#include <assert.h>
#include <limits.h>
#include <stdbool.h>

#include "profiling_timer.h"
#include "profiler.h"

#include "async_global.h"

void initialize_profiler(void)
{
    initialize_profiling_timer();
}

uint32_t elapsed_time(profiling_timer_data_t const *const start_time,
                      profiling_timer_data_t const *const end_time)
{
    assert(end_time->max_timer_value == start_time->max_timer_value);
    assert(end_time->overflow_count >= start_time->overflow_count);

    /* NOTE(bja, 2017-04) When overflow has occurred, end - start can */
    /* meaningfully be a negative value! */
    int32_t delta_time = end_time->timer_count - start_time->timer_count;
    int32_t delta_overflow = end_time->overflow_count - start_time->overflow_count;
    delta_time += delta_overflow * end_time->max_timer_value;
    /* NOTE(bja, 2017-04) trap signed/unsigned conversion overflow problem */
    if (delta_time > 40000000u) {
        /* missed a overflow somehow.... */
        increment_global_async_profiling_overflow();
        delta_time = end_time->timer_count - start_time->timer_count;
        delta_overflow = 1 + end_time->overflow_count - start_time->overflow_count;
        delta_time += delta_overflow * end_time->max_timer_value;
        if (delta_time > 40000000u) {
            bool inf_loop = true;
            while (inf_loop) {}
        }
    }
    assert(delta_time < 40000000u);

    return (uint32_t)delta_time;
}

