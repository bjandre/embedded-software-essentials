
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

#include "platform_defs.h"
#if (PLATFORM == PLATFORM_FRDM)
#include "profiling_timer_frdm_kl25z.h"
#elif (PLATFORM == PLATFORM_BBB)
#include "profiling_timer_bbb.h"
#include "compiler_compat.h"
#else
#include "profiling_timer_host.h"
#include "compiler_compat.h"
#endif

#include "profiling_timer_data.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

/**
  Platform independent wrapper routine to initialize the profiling timer
 */
__attribute__( ( always_inline ) ) __STATIC_INLINE void
initialize_profiling_timer(void)
{
#if (PLATFORM == PLATFORM_FRDM)
    frdm_kl25z_initialize_profiling_timer();
#elif (PLATFORM == PLATFORM_BBB)
    bbb_initialize_profiling_timer();
#else
    host_initialize_profiling_timer();
#endif
}

/**
  Platform independent wrapper routine to retreive clocks per second for the
  profiling timer.
 */
__attribute__( ( always_inline ) ) __STATIC_INLINE uint32_t
get_clocks_per_second_profiling_timer(void)
{
    uint32_t cps;
#if (PLATFORM == PLATFORM_FRDM)
    cps = frdm_kl25z_get_clocks_per_second_profiling_timer();
#elif (PLATFORM == PLATFORM_BBB)
    cps = bbb_get_clocks_per_second_profiling_timer();
#else
    cps = host_get_clocks_per_second_profiling_timer();
#endif
    return cps;
}

/**
  Platform indeendent wrapper routine to get the profiling timer value

  \param[out] current timer value
 */
__attribute__( ( always_inline ) ) __STATIC_INLINE void get_profiling_timer(
    profiling_timer_data_t *timer_data)
{
#if (PLATFORM == PLATFORM_FRDM)
    frdm_kl25z_get_profile_timer(timer_data);
#elif (PLATFORM == PLATFORM_BBB)
    bbb_get_profile_timer(timer_data);
#else
    host_get_profile_timer(timer_data);
#endif
}

#pragma GCC diagnostic pop

#endif/* ESE_HAL_PROFILING_TIMER_H_ */
