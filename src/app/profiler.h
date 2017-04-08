
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

/**
  Initialize the profiler
 */
void initialize_profiler(void);

/**
  Get the current timer value

  \param[out] timer_data timer struct to store data
 */
void get_timer(profiling_timer_data_t *timer_data);

/**
  Get the elapsed time since a start value

  \param[in] start_time starting timer value

  \return elapsed time since timer start
 */
uint32_t elapsed_time(profiling_timer_data_t *start_time);

#endif // ESE_APP_PROFILER_H_
