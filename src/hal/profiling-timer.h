
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef ESE_HAL_PROFILING_TIMER_H_
#define ESE_HAL_PROFILING_TIMER_H_

#include <stdint.h>
#include <time.h>

#include "platform-defs.h"
#if (PLATFORM == PLATFORM_FRDM)
#include "profiling-timer-frdm-kl25z.h"
#else
#include "profiling-timer-host.h"
#endif

#include "profiling-timer-data.h"
/*
  Platform independent wrapper routine to initialize the profiling timer
 */
__attribute__( ( always_inline ) ) static inline void
initialize_profiling_timer(void)
{
#if (PLATFORM == PLATFORM_FRDM)
    frdm_kl25z_initialize_profiling_timer();
#else
    host_initialize_profiling_timer();
#endif
}

/*
  Platform indeendent wrapper routine to get the profiling timer value

  \param[out] current timer value
 */
__attribute__( ( always_inline ) ) static inline void get_profiling_timer(
    profiling_timer_data_t *timer_data)
{
#if (PLATFORM == PLATFORM_FRDM)
    frdm_kl25z_get_profile_timer(timer_data);
#else
    host_get_profile_timer(timer_data);
#endif
}

#endif // ESE_HAL_PROFILING_TIMER_H_
