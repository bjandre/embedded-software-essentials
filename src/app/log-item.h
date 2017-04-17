
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef ESE_APP_LOG_ITEM_H_
#define ESE_APP_LOG_ITEM_H_

/**
   \file log-item.h

   Binary Logger implemenation. Implementation is flexible and works on a
   generic 'uart', which can be hardware uart or software (printf/getc).

   The logger is global and accessed in critical regions, e.g. interrupts.  All
   instances must be volatile.

 */

#include <stdint.h>
#include <time.h>

#include "logger.h"

/**
   Identifiers for data sent via the binary logger.
 */
typedef enum BinaryLoggerID {
    LOGGER_INITIALIZED,
    RTC_INITIALIZED,
    GPIO_INITIALIZED,
    DMA_INITIALIZED,
    SPI_INITIALIZED,
    SYSTEM_INITIALIZED,
    SYSTEM_HALTED,
    HEARTBEAT,
    INFO,
    WARNING,
    ERROR,
    POST_START,
    POST_ERROR,
    POST_STATUS,
    POST_NUM_TESTS_RUN,
    POST_NUM_TESTS_SKIPPED,
    POST_NUM_TESTS_PASSED,
    POST_COMPLETE,
    DATA_RECEIVED,
    DATA_ANALYSIS_STARTED,
    DATA_ALPHA_COUNT,
    DATA_NUMERIC_COUNT,
    DATA_PUNCTUATION_COUNT,
    DATA_MISC_COUNT,
    DATA_ANALYSIS_COMPLETED,
    PROFILING_START,
    PROFILING_VALUE,
    PROFILING_END,
} BinaryLoggerID;

/**
   Container for items sent to the binary logger.

   NOTE(bja, 2017-03) logged item is limited to a size of max_payload_bytes
*/
typedef struct BinaryLoggerItem {
    BinaryLoggerID id;
    time_t timestamp;
    logger_size_t payload_num_bytes;
    uint8_t *payload;
} log_item_t;

/**
   Initialize the logger in a way to make it self describing for log items.

     Now we need to send four bytes to make the logger self describing.

     1. two bytes - magic number 0x4477 in hex. Allows us to tell if we have
     one of our files, and whether we have big or little endian data.

     2. one byte - the number of bytes in the log_item_t id.

     3. one byte - the second is the number of bytes in the log_item_t
     payload_num_bytes.
 */
BinaryLoggerStatus InitializeLoggerForLogItems(void);

/**
   Allocate a log item and internal buffer.

   \param[out] item - return the location of the allocated item

   \return status of the operation
 */
BinaryLoggerStatus CreateLogItem(log_item_t **item);

/**
   Update the values stored in a previously allocated log item

   \param[in,out] *item - pointer to item storage
   \param[in] id - id of the item being stored
   \param[in] num_bytes - number of bytes in the payload
   \param[in] payload - pointer to the user supplied buffer containing the data

   \return status of the operation
 */
BinaryLoggerStatus UpdateLogItem(log_item_t *item, BinaryLoggerID id,
                                 logger_size_t num_bytes, const void *payload);

/**
   Update the values stored in a previously allocated log item with defaults for no payload.

   \param[in,out] *item - pointer to item storage
   \param[in] id - id of the item being stored

   \return status of the operation
 */
BinaryLoggerStatus UpdateLogItemNoPayload(log_item_t *item, BinaryLoggerID id);

/**
   Free the internal item memory and the item itself.

   \param[in,out] *item - pointer to item to be freed.

   \return status of the operation
 */
BinaryLoggerStatus DestroyLogItem(log_item_t **item);

/**
   Log the user provided item to the global logger.

   \param[in] *item - pointer to item to be logged

   \return status of the operation
 */
BinaryLoggerStatus log_item(const log_item_t *item);

#endif/* ESE_APP_LOG_ITEM_H_ */
