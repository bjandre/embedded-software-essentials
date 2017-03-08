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

typedef uint8_t logger_size_t;
static logger_size_t const max_payload_bytes = UINT8_MAX;
static const logger_size_t zero_payload_bytes = 0;
static const void *null_payload = NULL;

// NOTE(bja, 2017-03) log item is limited to a max of UINT8_MAX items
typedef struct BinaryLoggerData {
    BinaryLoggerID id;
    logger_size_t payload_num_bytes;
    uint8_t *payload;
} log_item_t;

BinaryLoggerStatus BinaryLoggerInitialize(logger_size_t num_bytes);

BinaryLoggerStatus log_data(logger_size_t num_bytes, uint8_t *buffer);
BinaryLoggerStatus log_string(uint8_t *string);
BinaryLoggerStatus log_integer(int32_t integer);
BinaryLoggerStatus log_flush(void);

BinaryLoggerStatus log_receive_data(logger_size_t num_bytes, uint8_t *buffer);

BinaryLoggerStatus CreateLogItem(log_item_t **item);
BinaryLoggerStatus UpdateLogItem(log_item_t *item, BinaryLoggerID id,
                                 logger_size_t num_bytes, const void *payload);
BinaryLoggerStatus DestroyLogItem(log_item_t **item);
BinaryLoggerStatus log_item(const log_item_t *item);


#endif // ESE_ARCH_LOGGER_H_
