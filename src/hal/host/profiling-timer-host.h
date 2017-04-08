
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

#include "profiling-timer-data.h"

/*
  Platform specific routine to initialize the profiling timer
 */
__attribute__( ( always_inline ) ) static inline void
host_initialize_profiling_timer(void)
{
}

/*
  Platform specific routine to get the profiling timer value

  \return timer value
 */
__attribute__( ( always_inline ) ) static inline void host_get_profile_timer(
    profiling_timer_data_t *timer_data)
{
    timer_data->timer_count = (uint32_t)clock();
    timer_data->overflow_count = 0;
    timer_data->timer_bytes = sizeof(clock_t);
}


#endif // ESE_HAL_HOST_PROFILING_TIMER_H_
