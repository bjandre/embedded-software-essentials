
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include <stdint.h>

#include "log_item.h"
#include "binary_logger.h"
#include "leds.h"
#include "heartbeat.h"

#include "async_global.h"

void heartbeat(log_item_t *item)
{
    bool heartbeat_occurred = get_global_async_heartbeat_occurred();
    if (heartbeat_occurred) {
        set_global_async_heartbeat_occurred(false);
        log_item_update_no_payload(item, HEARTBEAT);
        log_item(item);
        leds_heartbeat();
    }
}
