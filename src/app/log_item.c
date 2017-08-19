
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
#include <time.h>

#include "memory_common.h"
#include "memory_cpu.h"

#include "binary_logger.h"
#include "log_item.h"

#ifdef LOGGING_ENABLED

#include "async_global.h"

/* define some reusable constants */
static const logger_size_t max_payload_bytes =
    UINT8_MAX;/*!< maximum payload size in bytes */
static const logger_size_t zero_payload_bytes =
    0;/*!< consant for zero payload size */
static const void *null_payload = NULL;/*!< constant for a null payload */

binary_logger_status_t log_item_initialize_logger(void)
{
    binary_logger_status_t status = BINARY_LOGGER_SUCCESS;
    uint16_t magic_number = 0x4577u;
    binary_logger_log_data(sizeof(magic_number), (uint8_t *)(&magic_number));
    uint8_t data = sizeof(binary_logger_id_t);
    binary_logger_log_data(sizeof(data), (uint8_t *)(&data));
    data = sizeof(time_t);
    binary_logger_log_data(sizeof(data), (uint8_t *)(&data));
    data = sizeof(logger_size_t);
    binary_logger_log_data(sizeof(data), (uint8_t *)(&data));
    return status;
}


binary_logger_status_t log_item_create(log_item_t **item)
{
    binary_logger_status_t status = BINARY_LOGGER_SUCCESS;
    *item = malloc(sizeof(log_item_t));
    if (NULL == *item) {
        status = BINARY_LOGGER_ITEM_ALLOC_ERROR;
    } else {
        (*item)->id = 0;
        (*item)->payload_num_bytes = 0;
        (*item)->payload = malloc(max_payload_bytes);
        if (NULL == (*item)->payload) {
            status = BINARY_LOGGER_ITEM_ALLOC_ERROR;
        }
    }
    if (BINARY_LOGGER_SUCCESS != status) {
        log_item_destroy(item);
    }
    return status;
}

binary_logger_status_t log_item_update(log_item_t *item, binary_logger_id_t id,
                                       logger_size_t num_bytes, const void *payload)
{
    binary_logger_status_t status = BINARY_LOGGER_SUCCESS;
    if (NULL == item) {
        status = BINARY_LOGGER_NULL_POINTER;
    } else {
        if (NULL == item->payload) {
            status = BINARY_LOGGER_NULL_POINTER;
        } else {
            if (BINARY_LOGGER_SUCCESS == status) {
                item->id = id;
                item->timestamp = get_global_async_heartbeat_timestamp();
                /* FIXME(bja, 2017-03) what's the best error handling for a size greater */
                /* than our buffer...? Return an error, or just log what we can.... */
                num_bytes = num_bytes > max_payload_bytes ? max_payload_bytes : num_bytes;
                item->payload_num_bytes = num_bytes;
                if (item->payload_num_bytes > 0) {
                    MemStatus mem_stat = memmove_cpu(item->payload, (uint8_t *)payload,
                                                     item->payload_num_bytes);
                    if (MemStatus_Success != mem_stat) {
                        status = BINARY_LOGGER_ITEM_ALLOC_ERROR;
                    }
                }
            }
        }
    }
    return status;
}

binary_logger_status_t log_item_update_no_payload(log_item_t *item,
        binary_logger_id_t id)
{
    return log_item_update(item, id, zero_payload_bytes, null_payload);
}

binary_logger_status_t log_item(const log_item_t *item)
{
    binary_logger_status_t status = BINARY_LOGGER_SUCCESS;
    if (NULL == item) {
        status = BINARY_LOGGER_NULL_ITEM;
    } else {
        binary_logger_log_data(sizeof(item->id), (uint8_t *)(&(item->id)));
        binary_logger_log_data(sizeof(item->timestamp),
                               (uint8_t *)(&(item->timestamp)));
        binary_logger_log_data(sizeof(item->payload_num_bytes),
                               (uint8_t *)(&item->payload_num_bytes));
        if (item->payload_num_bytes > 0) {
            if (null_payload == item->payload) {
                status = BINARY_LOGGER_NULL_DATA;
            } else {
                binary_logger_log_data(item->payload_num_bytes, item->payload);
            }
        }
    }
    return status;
}

binary_logger_status_t log_item_destroy(log_item_t **item)
{
    binary_logger_status_t status = BINARY_LOGGER_SUCCESS;
    if (NULL != *item) {
        free((*item)->payload);
    }
    free(*item);
    *item = NULL;
    return status;
}

#else/* LOGGING_DISABLED - disable logging by providing empty functions that */
/* simply return status OK. These may be removed by the linker when link
   time optimization in enabled. If profiling shows that they are not, we
   can replace them with a macro that substitutes status ok. */
binary_logger_status_t log_item_create(log_item_t **item)
{
    (void)item;
    return BINARY_LOGGER_SUCCESS;
}

binary_logger_status_t log_item_update(log_item_t *item, binary_logger_id_t id,
                                       logger_size_t num_bytes, const void *payload)
{
    (void)item;
    (void)num_bytes;
    (void)payload;
    return BINARY_LOGGER_SUCCESS;
}

binary_logger_status_t log_item_destroy(log_item_t **item)
{
    (void)item;
    return BINARY_LOGGER_SUCCESS;
}

binary_logger_status_t log_item(const log_item_t *item)
{
    (void)item;
    return BINARY_LOGGER_SUCCESS;
}

#endif
