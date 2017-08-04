
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include <stdbool.h>
#include <stddef.h>
#include <time.h>

#include "rtc-bbb.h"

#include "async-global.h"

void bbb_initialize_rtc(void)
{
}

void RTC_Seconds_IRQHandler(void)
{
    /* NOTE: no status flag to clear */
    time_t current_time = time(NULL);
    set_global_async_heartbeat_timestamp(current_time);
    set_global_async_heartbeat_occurred(true);
}
