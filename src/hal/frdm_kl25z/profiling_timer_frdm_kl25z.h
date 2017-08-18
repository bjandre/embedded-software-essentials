
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef ESE_HAL_FRDM_KL25Z_PROFILING_TIMER_H_
#define ESE_HAL_FRDM_KL25Z_PROFILING_TIMER_H_

#include <stdint.h>
#include <time.h>

#include "MKL25Z4.h"

#include "profiling_timer_data.h"

#include "async_global.h"

/**
   Return the number of clocks per second for the profiling timer
 */
uint32_t frdm_kl25z_get_clocks_per_second_profiling_timer(void);

/**
  Platform specific routine to initialize the profiling timer
 */
void frdm_kl25z_initialize_profiling_timer(void);

/**
  Platform specific routine to get the profiling timer value

  \return timer value
 */
__attribute__( ( always_inline ) ) static inline void
frdm_kl25z_get_profile_timer(profiling_timer_data_t *timer_data)
{
    timer_data->timer_count = (uint32_t)(TPM0->CNT);
    timer_data->overflow_count = get_global_async_profiling_overflow();
    timer_data->max_timer_value = UINT16_MAX;
}


#endif // ESE_HAL_FRDM_KL25Z_PROFILING_TIMER_H_
