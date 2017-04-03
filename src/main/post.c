
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include <stdint.h>

#include "platform-defs.h"
#include "post-common.h"
#include "post.h"

#if (PLATFORM == PLATFORM_FRDM)
#include "post-dma-frdm-kl25z.h"
#endif

void power_on_self_tests(log_item_t *item)
{
    UpdateLogItemNoPayload(item, POST_START);
    log_item(item);

    POSTstatus status = POST_ALL_SKIPPED;
#if (PLATFORM == PLATFORM_FRDM)
    status = post_dma_memmove_1byte();
    if (POST_PASS != status) {
        const char name[] = "post_dma_memmov_1byte";
        UpdateLogItem(item, POST_ERROR, sizeof(name), (uint8_t *)name);
        log_item(item);
    }

    status = post_dma_memmove_4byte();
    if (POST_PASS != status) {
        const char name[] = "post_dma_memmove_4byte";
        UpdateLogItem(item, POST_ERROR, sizeof(name), (uint8_t *)name);
        log_item(item);
    }

    status = post_dma_memset_1byte();
    if (POST_PASS != status) {
        const char name[] = "post_dma_memset_1byte";
        UpdateLogItem(item, POST_ERROR, sizeof(name), (uint8_t *)name);
        log_item(item);
    }

    status = post_dma_memset_4byte();
    if (POST_PASS != status) {
        const char name[] = "post_dma_memset_4byte";
        UpdateLogItem(item, POST_ERROR, sizeof(name), (uint8_t *)name);
        log_item(item);
    }
#else
    // running on a host system.
    if (POST_ALL_SKIPPED == status) {
        const char name[] = "host system - all POST skipped";
        UpdateLogItem(item, POST_STATUS, sizeof(name), (uint8_t *)name);
        log_item(item);
    }
#endif

    UpdateLogItemNoPayload(item, POST_COMPLETE);
    log_item(item);
}
