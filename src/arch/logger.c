#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "platform-defs.h"
#if (PLATFORM == PLATFORM_FRDM)
#include "MKL25Z4.h"
#endif

#include "memory.h"
#include "data.h"
#include "circular_buffer.h"
#include "uart.h"
#include "logger.h"

#ifdef LOGGING_ENABLED

extern BinaryLogger_t volatile logger;

static const uint32_t debugger_baud = 115200u;
static const logger_size_t bytes_per_item = sizeof(uint8_t);

BinaryLoggerStatus BinaryLoggerInitialize(logger_size_t num_bytes)
{
    BinaryLoggerStatus status = BinaryLogger_OK;
    CircularBufferStatus cb_status;
    logger_size_t num_buffer_items = num_bytes / bytes_per_item;
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

    // Now we need to send four bytes to make the logger self describing.
    //
    // 1. two bytes - magic number 0x4477 in hex. Allows us to tell if we have
    // one of our files, and whether we have big or little endian data.
    //
    // 2. one byte - the number of bytes in the log_item_t id.
    //
    // 3. one byte - the second is the number of bytes in the log_item_t
    // payload_num_bytes.
    //
    uint16_t magic_number = 0x4477u;
    log_data(sizeof(magic_number), (uint8_t *)(&magic_number));
    uint8_t data = sizeof(BinaryLoggerID);
    log_data(sizeof(data), (uint8_t *)(&data));
    data = sizeof(logger_size_t);
    log_data(sizeof(data), (uint8_t *)(&data));
    return status;
}

BinaryLoggerStatus log_data(logger_size_t num_bytes, uint8_t *buffer)
{
    BinaryLoggerStatus status = BinaryLogger_OK;
    CircularBufferStatus cb_status;
    for (logger_size_t n = 0; n < num_bytes; n++) {
        bool is_full;
        do {
            // poll the transmit buffer to see if in can accept data.
            cb_status = CircularBufferIsFull(logger.transmit_buffer, &is_full);
        } while (is_full);
        cb_status = CircularBufferAddItem(logger.transmit_buffer, buffer + n);
        if (CB_No_Error != cb_status) {
            abort();
        }

        // Only use interrupts on platforms that support it and it was requested in
        // the build. Otherwise use polling.
#if (PLATFORM == PLATFORM_FRDM) && (LOGGER_ALGORITHM == LOGGER_INTERRUPTS)
        // make sure transmit buffer empty interrupt is on
        //UART0->C2 |= UART0_C2_TIE(1);
        if (UART0->S1 & UART0_S1_TDRE_MASK) {
            // transmit buffer empty, turn on interrupts
            UART0->C2 |= UART0_C2_TIE(1);
        }
#else // LOGGER_ALGORITHM == LOGGER_POLLING
        // no interrupts, just write all available data
        bool is_empty;
        cb_status = CircularBufferIsEmpty(logger.transmit_buffer, &is_empty);
        while (!is_empty) {
            uint8_t byte;
            cb_status = CircularBufferRemoveItem(logger.transmit_buffer, &byte);
            UartStatus uart_status = uart_status = logger.uart.transmit_byte(byte);

            cb_status = CircularBufferIsEmpty(logger.transmit_buffer, &is_empty);
        }
#endif
    }

    return status;
}

BinaryLoggerStatus log_string(uint8_t *string)
{
    BinaryLoggerStatus status = BinaryLogger_OK;
    size_t string_length = 0;
    while (*(string + string_length) != '\0') {
        string_length++;
    }
    log_data(string_length, (uint8_t *)string);
    return status;
}

BinaryLoggerStatus log_integer(int32_t integer)
{
    BinaryLoggerStatus status = BinaryLogger_OK;
    const size_t max_digits = 32;
    char string[max_digits];
    my_memset((uint8_t *)string, max_digits, '\0');
    uint32_t decimal_base = 10;
    my_itoa((int8_t *)string, integer, decimal_base);
    log_string((uint8_t *)string);
    return status;
}

BinaryLoggerStatus log_flush(void)
{
    BinaryLoggerStatus status = BinaryLogger_OK;
    CircularBufferStatus cb_status;

    bool is_empty = false;
    do {
        cb_status = CircularBufferIsEmpty(logger.transmit_buffer, &is_empty);
        if (CB_No_Error != cb_status) {
            status = BinaryLogger_Error;
            break;
        }
    } while (!is_empty);
    return status;
}

BinaryLoggerStatus log_receive_data(logger_size_t num_bytes, uint8_t *buffer)
{
    BinaryLoggerStatus status = BinaryLogger_OK;
    UartStatus uart_status = UART_Status_OK;
    CircularBufferStatus cb_status = CB_No_Error;

#if (PLATFORM == PLATFORM_FRDM) && (LOGGER_ALGORITHM == LOGGER_INTERRUPTS)
    // extraction from uart receive data register and pack into the circular
    // buffer is done by the UART0 interrupt handler
    (void)uart_status; // keep the compiler happy
#else // LOGGER_ALGORITHM == LOGGER_POLLING
    for (logger_size_t n = 0; n < num_bytes; n++) {
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
#endif
    // extract from circular buffer and pack into user buffer.
    logger_size_t num_received = 0;
    while (num_received < num_bytes) {
        uint8_t byte;
        cb_status = CircularBufferRemoveItem(logger.receive_buffer, (void *)(&byte));
        if (CB_No_Error == cb_status) {
            *(buffer + num_received) = byte;
            num_received++;
        }
        // FIXME(bja, 2017-03) error handling if we didn't get our desired number of bytes? just return what we got...
    }
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
BinaryLoggerStatus BinaryLoggerInitialize(logger_size_t num_bytes)
{
    (void)null_payload;
    (void)num_bytes;
    return BinaryLogger_OK;
}

BinaryLoggerStatus log_data(logger_size_t num_bytes, uint8_t *buffer)
{
    (void)num_bytes;
    (void)buffer;
    return BinaryLogger_OK;
}

BinaryLoggerStatus log_string(uint8_t *string)
{
    (void)string;
    return BinaryLogger_OK;
}

BinaryLoggerStatus log_integer(int32_t integer)
{
    (void)integer;
    return BinaryLogger_OK;
}

BinaryLoggerStatus log_flush(void)
{
    return BinaryLogger_OK;
}

BinaryLoggerStatus log_receive_data(logger_size_t num_bytes, uint8_t *buffer)
{
    (void)num_bytes;
    (void)buffer;
    return BinaryLogger_OK;
}

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
