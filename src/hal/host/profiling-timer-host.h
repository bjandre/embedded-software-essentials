
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef ESE_HAL_HOST_PROFILING_TIMER_H_
#define ESE_HAL_HOST_PROFILING_TIMER_H_

#include <stdint.h>
#include <time.h>

#include "compiler-compat.h"

#include "profiling-timer-data.h"

static uint32_t clock_t_max;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

/*
  Platform specific routine to initialize the profiling timer
 */
__attribute__( ( always_inline ) ) __STATIC_INLINE void
host_initialize_profiling_timer(void)
{
    /* FIXME(bja, 2017-04) assumes that clock_t is unsigned! */
    clock_t_max = 0;
    for (uint8_t n = 0; n < sizeof(clock_t); n += 2) {
        clock_t_max |= (0xFF << n);
    }
}

/*
  Platform specific routine to get the profiling timer value

  \return timer value
 */
__attribute__( ( always_inline ) ) __STATIC_INLINE void host_get_profile_timer(
    profiling_timer_data_t *timer_data)
{
    timer_data->timer_count = (uint32_t)clock();
    timer_data->overflow_count = 0;
    timer_data->max_timer_value = clock_t_max;
}

#pragma GCC diagnostic pop


#endif/* ESE_HAL_HOST_PROFILING_TIMER_H_ */
