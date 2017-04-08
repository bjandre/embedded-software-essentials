
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef ESE_HAL_PROFILING_TIMER_DATA_H_
#define ESE_HAL_PROFILING_TIMER_DATA_H_

#include <stdint.h>

/**
   state information for profiling timer
 */
typedef struct {
    uint32_t timer_count; //!< current timer count
    uint32_t overflow_count; //!< current overflow count
    uint8_t timer_bytes; //!< number of bytes in the timer
} profiling_timer_data_t;

#endif // ESE_HAL_PROFILING_TIMER_DATA_H_
