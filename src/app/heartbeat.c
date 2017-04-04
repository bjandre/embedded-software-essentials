
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include <stdint.h>

#include "log-item.h"
#include "logger.h"
#include "heartbeat.h"

#include "platform-defs.h"
#if (PLATFORM == PLATFORM_FRDM)
#include "gpio-frdm-kl25z.h"
#endif

void heartbeat(log_item_t *item, uint32_t heartbeat_timestamp)
{
    UpdateLogItem(item, HEARTBEAT, sizeof(heartbeat_timestamp),
                  &heartbeat_timestamp);
    log_item(item);
#if (PLATFORM == PLATFORM_FRDM)
    frdm_kl25z_heartbeat_led();
#endif
}
