
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef ESE_ARCH_LOGGER_H_
#define ESE_ARCH_LOGGER_H_

/**
   \file logger.h

   Binary Logger implemenation. Implementation is flexible and works on a
   generic 'uart', which can be hardware uart or software (printf/getc).

   The logger is global and accessed in critical regions, e.g. interrupts.  All
   instances must be volatile.

 */

#include <stddef.h>
#include <stdint.h>

#include "circular_buffer.h"
#include "uart.h"

/**
   Buffers and UART associated with the logger
 */
typedef struct BinaryLogger {
    CircularBuffer_t volatile *transmit_buffer;
    CircularBuffer_t volatile *receive_buffer;
    uart_t uart;
} BinaryLogger_t;

/**
   Status codes returned by binary logger operations
 */
typedef enum BinaryLoggerStatus {
    BinaryLogger_OK,
    BinaryLogger_Error,
    BinaryLogger_Null,
    BinaryLogger_ItemAllocationError,
    BinaryLogger_ItemNULL,
    BinaryLogger_DataNull,
} BinaryLoggerStatus;

/**
   define a type for for the sizes used to interact with the logger
*/
typedef uint8_t logger_size_t;

/**
   Initialize the global binary logger to the specified nuber of bytes

   \param[in] num_bytes - number of bytes in the send and receive buffers.

   \return status of the operation
 */
BinaryLoggerStatus BinaryLoggerInitialize(logger_size_t num_bytes);

/**
   Log the specified number of bytes from the provided buffer.

   \param[in] num_bytes - number of bytes to log
   \param[in] buffer - pointer to the data to be logged.

   \return status of the operation
 */
BinaryLoggerStatus log_data(logger_size_t num_bytes, uint8_t *buffer);

/**
   Log the null terminated c string.

   \param[in] string - pointer to the null terminated c string to be logged

   \return status of the operation
 */
BinaryLoggerStatus log_string(uint8_t *string);

/**
   Log the 32 bit integer

   \param[in] integer - integer to be logged

   \return status of the operation
 */
BinaryLoggerStatus log_integer(int32_t integer);


/**
   Poll the transmit buffer until it returns empty

   \return status of the operation
 */
BinaryLoggerStatus log_flush(void);

/**
   Extract data from the receive buffer and return it to the user.

   Polls the circular buffer until the requested number of bytes are
   available. To avoid polling, request one byte at a time.

   The global_async_data.data_available flag will be set when data is available
   in the receive buffer.

   \param[in] num_bytes - number of bytes to extract from the receive circular
                       buffer and return to the user.

   \param[out] buffer - pointer to user supplied buffer to store the data
                    in. Assumes that the buffer is large enough to hold the
                    requested number of bytes.

   \return status of the operation
 */
BinaryLoggerStatus log_receive_data(logger_size_t num_bytes, uint8_t *buffer);

/**
   Identifiers for data sent via the binary logger.
 */
typedef enum BinaryLoggerID {
    LOGGER_INITIALIZED,
    GPIO_INITIALIZED,
    SYSTEM_INITIALIZED,
    SYSTEM_HALTED,
    INFO,
    WARNING,
    ERROR,
    DATA_RECEIVED,
    DATA_ANALYSIS_STARTED,
    DATA_ALPHA_COUNT,
    DATA_NUMERIC_COUNT,
    DATA_PUNCTUATION_COUNT,
    DATA_MISC_COUNT,
    DATA_ANALYSIS_COMPLETED,
} BinaryLoggerID;

// define some reusable constants
static const logger_size_t max_payload_bytes =
    UINT8_MAX; //!< maximum payload size in bytes
static const logger_size_t zero_payload_bytes =
    0; //!< consant for zero payload size
static const void *null_payload = NULL; //!< constant for a null payload

/**
   Container for items sent to the binary logger.

   NOTE(bja, 2017-03) logged item is limited to a size of max_payload_bytes
*/
typedef struct BinaryLoggerItem {
    BinaryLoggerID id;
    logger_size_t payload_num_bytes;
    uint8_t *payload;
} log_item_t;

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


#endif // ESE_ARCH_LOGGER_H_
