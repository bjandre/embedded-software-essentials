
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include <stdint.h>
#include <stdlib.h>

#include "memory.h"

#include "logger.h"
#include "log-item.h"

#ifdef LOGGING_ENABLED

BinaryLoggerStatus InitializeLoggerForLogItems(void)
{
    BinaryLoggerStatus status = BinaryLogger_OK;
    uint16_t magic_number = 0x4477u;
    log_data(sizeof(magic_number), (uint8_t *)(&magic_number));
    uint8_t data = sizeof(BinaryLoggerID);
    log_data(sizeof(data), (uint8_t *)(&data));
    data = sizeof(logger_size_t);
    log_data(sizeof(data), (uint8_t *)(&data));
    return status;
}


BinaryLoggerStatus CreateLogItem(log_item_t **item)
{
    BinaryLoggerStatus status = BinaryLogger_OK;
    *item = malloc(sizeof(log_item_t));
    if (NULL == *item) {
        status = BinaryLogger_ItemAllocationError;
    } else {
        (*item)->id = 0;
        (*item)->payload_num_bytes = 0;
        (*item)->payload = malloc(max_payload_bytes);
        if (NULL == (*item)->payload) {
            status = BinaryLogger_ItemAllocationError;
        }
    }
    if (BinaryLogger_OK != status) {
        DestroyLogItem(item);
    }
    return status;
}

BinaryLoggerStatus UpdateLogItem(log_item_t *item, BinaryLoggerID id,
                                 logger_size_t num_bytes, const void *payload)
{
    BinaryLoggerStatus status = BinaryLogger_OK;
    if (NULL == item) {
        status = BinaryLogger_Null;
    }

    if (NULL == item->payload) {
        status = BinaryLogger_Null;
    }

    if (BinaryLogger_OK == status) {
        item->id = id;
        // FIXME(bja, 2017-03) what's the best error handling for a size greater
        // than our buffer...? Return an error, or just log what we can....
        num_bytes = num_bytes > max_payload_bytes ? max_payload_bytes : num_bytes;
        item->payload_num_bytes = num_bytes;
        if (item->payload_num_bytes > 0) {
            MemStatus mem_stat = my_memmove((uint8_t *)payload, item->payload,
                                            item->payload_num_bytes);
            if (MemStatus_SUCCESS != mem_stat) {
                status = BinaryLogger_ItemAllocationError;
            }
        }
    }
    return status;
}

BinaryLoggerStatus log_item(const log_item_t *item)
{
    BinaryLoggerStatus status = BinaryLogger_OK;
    if (NULL == item) {
        status = BinaryLogger_ItemNULL;
    } else {
        log_data(sizeof(item->id), (uint8_t *)(&(item->id)));
        log_data(sizeof(item->payload_num_bytes),
                 (uint8_t *)(&item->payload_num_bytes));
        if (item->payload_num_bytes > 0) {
            if (null_payload == item->payload) {
                status = BinaryLogger_DataNull;
            } else {
                log_data(item->payload_num_bytes, item->payload);
            }
        }
    }
    return status;
}

BinaryLoggerStatus DestroyLogItem(log_item_t **item)
{
    BinaryLoggerStatus status = BinaryLogger_OK;
    if (NULL != *item) {
        free((*item)->payload);
    }
    free(*item);
    *item = NULL;
    return status;
}

#else // LOGGING_DISABLED - disable logging by providing empty functions that
// simply return status OK. These may be removed by the linker when link
// time optimization in enabled. If profiling shows that they are not, we
// can replace them with a macro that substitutes status ok.
BinaryLoggerStatus CreateLogItem(log_item_t **item)
{
    (void)item;
    return BinaryLogger_OK;
}

BinaryLoggerStatus UpdateLogItem(log_item_t *item, BinaryLoggerID id,
                                 logger_size_t num_bytes, const void *payload)
{
    (void)item;
    (void)num_bytes;
    (void)payload;
    return BinaryLogger_OK;
}

BinaryLoggerStatus DestroyLogItem(log_item_t **item)
{
    (void)item;
    return BinaryLogger_OK;
}

BinaryLoggerStatus log_item(const log_item_t *item)
{
    (void)item;
    return BinaryLogger_OK;
}

#endif
