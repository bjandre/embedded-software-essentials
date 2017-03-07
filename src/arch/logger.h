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
    BinaryLogger_ItemAllocationError,
    BinaryLogger_ItemNULL,
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

typedef struct BinaryLoggerData {
    BinaryLoggerID id;
    size_t payload_num_bytes;
    void *payload;
} log_item_t;

BinaryLoggerStatus BinaryLoggerInitialize(size_t num_bytes);

BinaryLoggerStatus log_data(size_t num_bytes, uint8_t *buffer);
BinaryLoggerStatus log_string(uint8_t *string);
BinaryLoggerStatus log_integer(int32_t integer);
BinaryLoggerStatus log_flush(void);

BinaryLoggerStatus log_receive_data(size_t num_bytes, uint8_t *buffer);

BinaryLoggerStatus CreateLogItem(log_item_t **item, BinaryLoggerID id,
                                 size_t num_bytes, void *payload);
BinaryLoggerStatus DestroyLogItem(log_item_t **item);
BinaryLoggerStatus log_item(log_item_t *item);


#endif // ESE_ARCH_LOGGER_H_
