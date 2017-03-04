#ifndef ESE_ARCH_LOGGER_H_
#define ESE_ARCH_LOGGER_H_

#include <stddef.h>
#include <stdint.h>

typedef enum BinaryLoggerStatus {
    BinaryLogger_OK,
    BinaryLogger_Error,
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
} log_data_t;

BinaryLoggerStatus BinaryLoggerInitialize(size_t num_bytes);

BinaryLoggerStatus log_data(size_t num_bytes, uint8_t* buffer);
BinaryLoggerStatus log_string(char *string);
BinaryLoggerStatus log_integer(uint32_t integer);
BinaryLoggerStatus log_flush(void);

BinaryLoggerStatus log_receive_data(size_t num_bytes, uint8_t* buffer);

BinaryLoggerStatus create_log_item(log_data_t *log_data, BinaryLoggerID id, size_t num_bytes, void *payload);

BinaryLoggerStatus log_item(log_data_t *log_data);

BinaryLoggerStatus destroy_log_item(log_data_t *log_data);

#endif // ESE_ARCH_LOGGER_H_
