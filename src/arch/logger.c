#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "platform-defs.h"
#include "circular_buffer.h"
#include "uart.h"
#include "logger.h"

static const uint32_t debugger_baud = 115200u;
static const size_t num_buffer_items = 64;
static const size_t bytes_per_item = sizeof(uint8_t);

typedef struct BinaryLogger {
    CircularBuffer_t *transmit_buffer;
    CircularBuffer_t *receive_buffer;
    uart_t uart;
} BinaryLogger_t;

static BinaryLogger_t logger;

BinaryLoggerStatus BinaryLoggerInitialize(size_t num_bytes)
{
    BinaryLoggerStatus status = BinaryLogger_OK;
    CircularBufferStatus cb_status;
    cb_status = CircularBufferNew(&(logger.transmit_buffer), num_buffer_items,
                                  bytes_per_item);
    if (CB_No_Error != cb_status) {
        return BinaryLogger_Error;
    }
    cb_status = CircularBufferNew(&(logger.receive_buffer), num_buffer_items,
                                  bytes_per_item);
    if (CB_No_Error != cb_status) {
        return BinaryLogger_Error;
    }
    UartStatus uart_status = CreateUART(&(logger.uart), UartDebugger);
    if (UART_Status_OK != uart_status) {
        abort();
    }
    logger.uart.initialize(debugger_baud);
    if (UART_Status_OK != uart_status) {
        abort();
    }
    return status;
}

BinaryLoggerStatus log_data(size_t num_bytes, uint8_t *buffer)
{
    BinaryLoggerStatus status = BinaryLogger_OK;
    UartStatus uart_status;
    CircularBufferStatus cb_status;
    for (size_t n = 0; n < num_bytes; n++) {
        bool is_full;
        do {
            // poll the transmit buffer to see if in can accept data.
            cb_status = CircularBufferIsFull(logger.transmit_buffer, &is_full);
        } while (is_full);
        cb_status = CircularBufferAddItem(logger.transmit_buffer, buffer + n);
        if (CB_No_Error != cb_status) {
            abort();
        }
    }
#if (PLATFORM == PLATFORM_FRDM)
    // eventually handled by interrupts
    bool is_empty;
    cb_status = CircularBufferIsEmpty(logger.transmit_buffer, &is_empty);
    while (!is_empty) {
        uint8_t byte;
        cb_status = CircularBufferRemoveItem(logger.transmit_buffer, &byte);
        uart_status = logger.uart.transmit_byte(byte);

        cb_status = CircularBufferIsEmpty(logger.transmit_buffer, &is_empty);
    }
#else // PLATFORM == host || PLATFORM == bbb
    // no interrupts, just write all available data
    bool is_empty;
    cb_status = CircularBufferIsEmpty(logger.transmit_buffer, &is_empty);
    while (!is_empty) {
        uint8_t byte;
        cb_status = CircularBufferRemoveItem(logger.transmit_buffer, &byte);
        uart_status = logger.uart.transmit_byte(byte);

        cb_status = CircularBufferIsEmpty(logger.transmit_buffer, &is_empty);
    }
#endif
    if (UART_Status_OK != uart_status) {
        status = BinaryLogger_Error;
    }
    return status;
}

BinaryLoggerStatus log_string(char *string)
{
    BinaryLoggerStatus status = BinaryLogger_Error;
    return status;
}

BinaryLoggerStatus log_integer(uint32_t integer)
{
    BinaryLoggerStatus status = BinaryLogger_Error;
    return status;
}

BinaryLoggerStatus log_flush(void)
{
    BinaryLoggerStatus status = BinaryLogger_Error;
    return status;
}

BinaryLoggerStatus log_receive_data(size_t num_bytes, uint8_t *buffer)
{
    // FIXME(bja, 2017-03) need to figure out how to do this with
    // interrupts.... Poll circular buffer until number of bytes are present...?
    BinaryLoggerStatus status = BinaryLogger_OK;
    UartStatus uart_status;
    CircularBufferStatus cb_status = CB_No_Error;

    // extract from uart and pack into the circular buffer. This code goes into
    // the interrupts....
    for (int n = 0; n < num_bytes; n++) {
        uint8_t byte;
        uart_status = logger.uart.receive_byte(&byte);
        if (UART_Status_OK != uart_status) {
            status = BinaryLogger_Error;
        }
        cb_status = CircularBufferAddItem(logger.receive_buffer, &byte);
        if (CB_No_Error != cb_status) {
            abort();
        }
    }

    // extract from circular buffer and pack into user buffer.
    for (int n = 0; n < num_bytes; n++) {
        uint8_t byte;
        cb_status = CircularBufferRemoveItem(logger.receive_buffer, (void *)(&byte));
        if (CB_No_Error == cb_status) {
            *(buffer + n) = byte;
        } else {
            abort();
        }
    }
    return status;
}
