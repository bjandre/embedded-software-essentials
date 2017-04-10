
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include <stdint.h>

#include "profiler.h"

#include "post-common.h"
#include "post-profiler.h"

POSTstatus post_profiler_nop()
{
    /** test the profiler for a single no-op.

        Too much inconsistency to report direct values for testing, so we need
        to just check a range.
    */
    profiling_timer_data_t start_time, end_time;
    get_timer(&start_time);
    __asm("NOP");
    get_timer(&end_time);
    uint32_t profile_time = elapsed_time(&start_time, &end_time);
    POSTstatus status = POST_PASS;
    if (profile_time > 5 || profile_time < 1) {
        status = POST_FAIL;
    }
    return status;
}


