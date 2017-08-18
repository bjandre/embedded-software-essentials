
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "platform_defs.h"

#ifdef LOGGING_ENABLED

#include "memory_cpu.h"
#include "data.h"
#include "circular_buffer.h"
#include "communication_peripheral.h"
#include "communication_peripheral_factory.h"
#include "logger.h"
#include "async_global.h"

/**
   logger transmit using polling
 */
void logger_polling_transmit_byte(void);

extern volatile async_data_t global_async_data;

static const uint32_t debugger_baud = 115200u;/*!< UART debugger baud */
static const logger_size_t bytes_per_item = sizeof(
            uint8_t);/*!< number of bytes per sent via logger */

BinaryLoggerStatus BinaryLoggerCreate(logger_size_t num_bytes)
{
    BinaryLoggerStatus status = BinaryLogger_Success;
    CircularBufferStatus cb_status;
    logger_size_t num_buffer_items = num_bytes / bytes_per_item;

    BinaryLogger_t *logger = malloc(sizeof(BinaryLogger_t));
    cb_status = CircularBufferNew(&(logger->transmit_buffer),
                                  num_buffer_items,
                                  bytes_per_item);
    if (CircularBuffer_Success != cb_status) {
        return BinaryLogger_Error;
    }
    cb_status = CircularBufferNew(&(logger->receive_buffer),
                                  num_buffer_items,
                                  bytes_per_item);
    if (CircularBuffer_Success != cb_status) {
        return BinaryLogger_Error;
    }

    CommStatus comm_status = CommCreate(&(logger->comm),
                                        UART_Logger);
    if (Comm_Status_Success != comm_status) {
        abort();
    }
    logger->comm.initialize(debugger_baud);
    if (Comm_Status_Success != comm_status) {
        abort();
    }

    set_global_async_logger(logger);
    return status;
}

void BinaryLoggerDestroy(void)
{
    {
        uint32_t interrupt_state = start_critical_region();
        BinaryLogger_t *logger = global_async_data.logger;
        if (NULL != logger) {
            CircularBufferDestroy(&(logger->transmit_buffer));
            CircularBufferDestroy(&(logger->receive_buffer));
            CommDestroy(&(logger->comm));
        }
        end_critical_region(interrupt_state);
    }
}

BinaryLoggerStatus log_data(logger_size_t num_bytes, uint8_t *buffer)
{
    BinaryLoggerStatus status = BinaryLogger_Success;
    CircularBufferStatus cb_status;
    uint32_t interrupt_state;
    for (logger_size_t n = 0; n < num_bytes; n++) {
        bool is_full = false;
        do {
            /* poll the transmit buffer to see if in can accept data. */
            {
                interrupt_state = start_critical_region();
                cb_status = CircularBufferIsFull(global_async_data.logger->transmit_buffer,
                                                 &is_full);
                end_critical_region(interrupt_state);
                if (CircularBuffer_Success != cb_status) {
                    abort();
                }

            }
        } while (is_full);
        {
            interrupt_state = start_critical_region();
            cb_status = CircularBufferAddItem(global_async_data.logger->transmit_buffer,
                                              buffer + n);
            end_critical_region(interrupt_state);
        }
        if (CircularBuffer_Success != cb_status) {
            abort();
        }

        /* Only use interrupts on platforms that support it and it was requested in */
        /* the build. Otherwise use polling. */
#if (PLATFORM == PLATFORM_FRDM) && (LOGGER_ALGORITHM == LOGGER_INTERRUPTS)
        {
            interrupt_state = start_critical_region();
            global_async_data.logger->comm.begin_async_transmit();
            end_critical_region(interrupt_state);
        }
#else/* LOGGER_ALGORITHM == LOGGER_POLLING */
        logger_polling_transmit_byte();
#endif
    }

    return status;
}

void logger_polling_transmit_byte(void)
{
    /* no interrupts, just write all available data */
    CircularBufferStatus cb_status;
    bool is_empty = false;
    uint32_t interrupt_state;
    {
        interrupt_state = start_critical_region();
        cb_status = CircularBufferIsEmpty(global_async_data.logger->transmit_buffer,
                                          &is_empty);
        end_critical_region(interrupt_state);
    }
    if (CircularBuffer_Success != cb_status) {
        abort();
    }
    while (!is_empty) {
        uint8_t byte;
        {
            interrupt_state = start_critical_region();
            cb_status = CircularBufferRemoveItem(global_async_data.logger->transmit_buffer,
                                                 &byte);
            if (CircularBuffer_Success == cb_status) {
                global_async_data.logger->comm.transmit_byte(byte);
            }

            CircularBufferIsEmpty(global_async_data.logger->transmit_buffer, &is_empty);
            end_critical_region(interrupt_state);
        }
    }
}
BinaryLoggerStatus log_string(uint8_t *string)
{
    BinaryLoggerStatus status = BinaryLogger_Success;
    size_t string_length = 0;
    while (*(string + string_length) != '\0') {
        string_length++;
    }
    log_data(string_length, (uint8_t *)string);
    return status;
}

BinaryLoggerStatus log_integer(int32_t integer)
{
    BinaryLoggerStatus status = BinaryLogger_Success;
    const size_t max_digits = 32;
    char string[max_digits];
    const uint8_t null_char = '\0';
    memset_cpu((uint8_t *)string, &null_char, max_digits);
    uint32_t decimal_base = 10;
    my_itoa((int8_t *)string, integer, decimal_base);
    log_string((uint8_t *)string);
    return status;
}

BinaryLoggerStatus log_flush(void)
{
    BinaryLoggerStatus status = BinaryLogger_Success;
    CircularBufferStatus cb_status;

    bool is_empty = false;
    do {
        {
            uint32_t interrupt_state = start_critical_region();
            cb_status = CircularBufferIsEmpty(global_async_data.logger->transmit_buffer,
                                              &is_empty);
            end_critical_region(interrupt_state);
        }
        if (CircularBuffer_Success != cb_status) {
            status = BinaryLogger_Error;
            break;
        }
    } while (!is_empty);
    return status;
}

BinaryLoggerStatus log_receive_data(logger_size_t num_bytes, uint8_t *buffer)
{
    BinaryLoggerStatus status = BinaryLogger_Success;
    CircularBufferStatus cb_status = CircularBuffer_Success;

#if (PLATFORM == PLATFORM_FRDM) && (LOGGER_ALGORITHM == LOGGER_INTERRUPTS)
    /* extraction from comm receive data register and pack into the circular */
    /* buffer is done by the communication peripheral interrupt handler */
#else/* LOGGER_ALGORITHM == LOGGER_POLLING */
    status = logger_polling_receive(num_bytes);
#endif
    /* extract from circular buffer and pack into user buffer. */
    logger_size_t num_received = 0;
    while (num_received < num_bytes) {
        uint8_t byte;
        {
            uint32_t interrupt_state = start_critical_region();
            cb_status = CircularBufferRemoveItem(global_async_data.logger->receive_buffer,
                                                 (void *)(&byte));
            end_critical_region(interrupt_state);
        }
        if (CircularBuffer_Success == cb_status) {
            *(buffer + num_received) = byte;
            num_received++;
        } else { /* (CircularBuffer_Success != cb_status) */
            abort();
        }
        /* FIXME(bja, 2017-03) error handling if we didn't get our desired number of bytes? just return what we got... */
    }
    return status;
}

BinaryLoggerStatus logger_polling_receive(logger_size_t num_bytes)
{
    BinaryLoggerStatus status = BinaryLogger_Success;
    CircularBufferStatus cb_status = CircularBuffer_Success;
    for (logger_size_t n = 0; n < num_bytes; n++) {
        uint8_t byte;
        {
            uint32_t interrupt_state = start_critical_region();
            CommStatus comm_status = global_async_data.logger->comm.receive_byte(&byte);
            if (Comm_Status_Success != comm_status) {
                status = BinaryLogger_Error;
            } else {
                cb_status = CircularBufferAddItem(global_async_data.logger->receive_buffer,
                                                  &byte);
            }
            end_critical_region(interrupt_state);
        }
        if (CircularBuffer_Success != cb_status) {
            abort();
        }
    }
    return status;
}

#else/* LOGGING_DISABLED - disable logging by providing empty functions that */
// simply return status OK. These may be removed by the linker when link
// time optimization in enabled. If profiling shows that they are not, we
// can replace them with a macro that substitutes status ok.
BinaryLoggerStatus BinaryLoggerCreate(logger_size_t num_bytes)
{
    (void)num_bytes;
    return BinaryLogger_Success;
}

BinaryLoggerStatus log_data(logger_size_t num_bytes, uint8_t *buffer)
{
    (void)num_bytes;
    (void)buffer;
    return BinaryLogger_Success;
}

BinaryLoggerStatus log_string(uint8_t *string)
{
    (void)string;
    return BinaryLogger_Success;
}

BinaryLoggerStatus log_integer(int32_t integer)
{
    (void)integer;
    return BinaryLogger_Success;
}

BinaryLoggerStatus log_flush(void)
{
    return BinaryLogger_Success;
}

BinaryLoggerStatus log_receive_data(logger_size_t num_bytes, uint8_t *buffer)
{
    (void)num_bytes;
    (void)buffer;
    return BinaryLogger_Success;
}
#endif
