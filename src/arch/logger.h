#ifndef ESE_ARCH_LOGGER_H_
#define ESE_ARCH_LOGGER_H_

/**

   Binary Logger implemenation. Implementation is flexible and works on a
   generic 'uart', which can be hardware uart or software (printf/getc).

   The logger is global and accessed in critical regions, e.g. interrupts.  All
   instances must be volatile.

 */

#include <stddef.h>
#include <stdint.h>

#include "circular_buffer.h"
#include "uart.h"

typedef struct BinaryLogger {
    CircularBuffer_t volatile *transmit_buffer;
    CircularBuffer_t volatile *receive_buffer;
    uart_t uart;
} BinaryLogger_t;

typedef enum BinaryLoggerStatus {
    BinaryLogger_OK,
    BinaryLogger_Error,
    BinaryLogger_Null,
    BinaryLogger_ItemAllocationError,
    BinaryLogger_ItemNULL,
    BinaryLogger_DataNull,
} BinaryLoggerStatus;

// define a type for for the sizes used to interact with the logger
typedef uint8_t logger_size_t;

/**
   BinaryLoggerInitialize()

   Initialize the global binary logger to the specified nuber of bytes

   Params: num_bytes - number of bytes in the send and receive buffers.

   Returns: status of the operation
 */
BinaryLoggerStatus BinaryLoggerInitialize(logger_size_t num_bytes);

/**
   log_data()

   Log the specified number of bytes from the provided buffer.

   Params: num_bytes - number of bytes to log
           buffer - pointer to the data to be logged.

   Returns: status of the operation
 */
BinaryLoggerStatus log_data(logger_size_t num_bytes, uint8_t *buffer);

/**
   log_string()

   Log the null terminated c string.

   Params: string - pointer to the string to be logged

   Returns: status of the operation
 */
BinaryLoggerStatus log_string(uint8_t *string);

/**
   log_integer()

   Log the 32 bit integer

   Params: integer - integer to be logged

   Returns: status of the operation
 */
BinaryLoggerStatus log_integer(int32_t integer);


/**
   log_flush()

   Poll the transmit buffer until it returns empty

   Returns: status of the operation
 */
BinaryLoggerStatus log_flush(void);

/**
   log_received_data()

   Extract data from the receive buffer and return it to the user.

   Polls the circular buffer until the requested number of bytes are
   available. To avoid polling, request one byte at a time.

   The global_async_data.data_available flag will be set when data is available
   in the receive buffer.

   Params: num_bytes - number of bytes to extract from the receive circular
                       buffer and return to the user.

           buffer - pointer to user supplied buffer to store the data
                    in. Assumes that the buffer is large enough to hold the
                    requested number of bytes.

   Returns: status of the operation
 */
BinaryLoggerStatus log_receive_data(logger_size_t num_bytes, uint8_t *buffer);

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
static const logger_size_t max_payload_bytes = UINT8_MAX;
static const logger_size_t zero_payload_bytes = 0;
static const void *null_payload = NULL;

// NOTE(bja, 2017-03) logged item is limited to a size of max_payload_bytes
typedef struct BinaryLoggerItem {
    BinaryLoggerID id;
    logger_size_t payload_num_bytes;
    uint8_t *payload;
} log_item_t;

/**
   CreateLogItem()

   Allocate a log item and internal buffer.

   Params: item - return the location of the allocated item

   Returns: status of the operation
 */
BinaryLoggerStatus CreateLogItem(log_item_t **item);

/**
   UpdateLogItem()

   Update the values stored in a previously allocated log item

   Params: *item - pointer to item storage
           id - id of the item being stored
           num_bytes - number of bytes in the payload
           payload - pointer to the user supplied buffer containing the data

   Returns: status of the operation
 */
BinaryLoggerStatus UpdateLogItem(log_item_t *item, BinaryLoggerID id,
                                 logger_size_t num_bytes, const void *payload);

/**
   DestroyLogItem()

   Free the internal item memory and the item itself.

   Params: *item - pointer to item to be freed.

   Returns: status of the operation
 */
BinaryLoggerStatus DestroyLogItem(log_item_t **item);

/**
   log_item()

   Log the user provided item to the global logger.

   Params: *item - pointer to item to be logged

   Returns: status of the operation
 */
BinaryLoggerStatus log_item(const log_item_t *item);


#endif // ESE_ARCH_LOGGER_H_
