
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "memory-common.h"
#include "memory-cpu.h"

#include "logger.h"
#include "log-item.h"

#ifdef LOGGING_ENABLED

// define some reusable constants
static const logger_size_t max_payload_bytes =
    UINT8_MAX; //!< maximum payload size in bytes
static const logger_size_t zero_payload_bytes =
    0; //!< consant for zero payload size
static const void *null_payload = NULL; //!< constant for a null payload

BinaryLoggerStatus InitializeLoggerForLogItems(void)
{
    BinaryLoggerStatus status = BinaryLogger_Success;
    uint16_t magic_number = 0x4577u;
    log_data(sizeof(magic_number), (uint8_t *)(&magic_number));
    uint8_t data = sizeof(BinaryLoggerID);
    log_data(sizeof(data), (uint8_t *)(&data));
    data = sizeof(logger_size_t);
    log_data(sizeof(data), (uint8_t *)(&data));
    return status;
}


BinaryLoggerStatus CreateLogItem(log_item_t **item)
{
    BinaryLoggerStatus status = BinaryLogger_Success;
    *item = malloc(sizeof(log_item_t));
    if (NULL == *item) {
        status = BinaryLogger_Item_Alloc_Error;
    } else {
        (*item)->id = 0;
        (*item)->payload_num_bytes = 0;
        (*item)->payload = malloc(max_payload_bytes);
        if (NULL == (*item)->payload) {
            status = BinaryLogger_Item_Alloc_Error;
        }
    }
    if (BinaryLogger_Success != status) {
        DestroyLogItem(item);
    }
    return status;
}

BinaryLoggerStatus UpdateLogItem(log_item_t *item, BinaryLoggerID id,
                                 logger_size_t num_bytes, const void *payload)
{
    BinaryLoggerStatus status = BinaryLogger_Success;
    if (NULL == item) {
        status = BinaryLogger_Null_Pointer;
    }

    if (NULL == item->payload) {
        status = BinaryLogger_Null_Pointer;
    }

    if (BinaryLogger_Success == status) {
        item->id = id;
        // FIXME(bja, 2017-03) what's the best error handling for a size greater
        // than our buffer...? Return an error, or just log what we can....
        num_bytes = num_bytes > max_payload_bytes ? max_payload_bytes : num_bytes;
        item->payload_num_bytes = num_bytes;
        if (item->payload_num_bytes > 0) {
            MemStatus mem_stat = memmove_cpu(item->payload, (uint8_t *)payload,
                                             item->payload_num_bytes);
            if (MemStatus_Success != mem_stat) {
                status = BinaryLogger_Item_Alloc_Error;
            }
        }
    }
    return status;
}

BinaryLoggerStatus UpdateLogItemNoPayload(log_item_t *item, BinaryLoggerID id)
{
    return UpdateLogItem(item, id, zero_payload_bytes, null_payload);
}

BinaryLoggerStatus log_item(const log_item_t *item)
{
    BinaryLoggerStatus status = BinaryLogger_Success;
    if (NULL == item) {
        status = BinaryLogger_Null_Item;
    } else {
        log_data(sizeof(item->id), (uint8_t *)(&(item->id)));
        log_data(sizeof(item->payload_num_bytes),
                 (uint8_t *)(&item->payload_num_bytes));
        if (item->payload_num_bytes > 0) {
            if (null_payload == item->payload) {
                status = BinaryLogger_Null_Data;
            } else {
                log_data(item->payload_num_bytes, item->payload);
            }
        }
    }
    return status;
}

BinaryLoggerStatus DestroyLogItem(log_item_t **item)
{
    BinaryLoggerStatus status = BinaryLogger_Success;
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
    return BinaryLogger_Success;
}

BinaryLoggerStatus UpdateLogItem(log_item_t *item, BinaryLoggerID id,
                                 logger_size_t num_bytes, const void *payload)
{
    (void)item;
    (void)num_bytes;
    (void)payload;
    return BinaryLogger_Success;
}

BinaryLoggerStatus DestroyLogItem(log_item_t **item)
{
    (void)item;
    return BinaryLogger_Success;
}

BinaryLoggerStatus log_item(const log_item_t *item)
{
    (void)item;
    return BinaryLogger_Success;
}

#endif
