
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef ESE_APP_PROFILER_H_
#define ESE_APP_PROFILER_H_

#include "profiling-timer-data.h"
#include "profiling-timer.h"

/**
  Initialize the profiler
 */
void initialize_profiler(void);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

/**
  Get the current timer value

  \param[out] timer_data timer struct to store data
 */
__attribute__( ( always_inline ) ) __STATIC_INLINE
void get_timer(profiling_timer_data_t *timer_data)
{
    get_profiling_timer(timer_data);
}
#pragma GCC diagnostic pop

/**
  Get the elapsed time since a start value

  \param[in] start_time starting timer value
  \param[in] end_time ending timer value

  \return elapsed time since timer start
 */
uint32_t elapsed_time(profiling_timer_data_t const *const start_time,
                      profiling_timer_data_t const *const end_time);

#endif/* ESE_APP_PROFILER_H_ */
