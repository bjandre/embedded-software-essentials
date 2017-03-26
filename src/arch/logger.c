
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

#include "platform-defs.h"
#if (PLATFORM == PLATFORM_FRDM)
#include "MKL25Z4.h"
#endif

#ifdef LOGGING_ENABLED

#include "memory-cpu.h"
#include "data.h"
#include "circular_buffer.h"
#include "uart.h"
#include "logger.h"

extern BinaryLogger_t volatile
logger; //!< extern reference to the global logger

static const uint32_t debugger_baud = 115200u; //!< UART debugger baud
static const logger_size_t bytes_per_item = sizeof(
            uint8_t); //!< number of bytes per sent via logger

BinaryLoggerStatus BinaryLoggerCreate(logger_size_t num_bytes)
{
    BinaryLoggerStatus status = BinaryLogger_Success;
    CircularBufferStatus cb_status;
    logger_size_t num_buffer_items = num_bytes / bytes_per_item;
    cb_status = CircularBufferNew(&(logger.transmit_buffer), num_buffer_items,
                                  bytes_per_item);
    if (CircularBuffer_Success != cb_status) {
        return BinaryLogger_Error;
    }
    cb_status = CircularBufferNew(&(logger.receive_buffer), num_buffer_items,
                                  bytes_per_item);
    if (CircularBuffer_Success != cb_status) {
        return BinaryLogger_Error;
    }

    UartStatus uart_status = CreateUART(&(logger.uart), UartDebugger);
    if (UART_Status_Success != uart_status) {
        abort();
    }
    logger.uart.initialize(debugger_baud);
    if (UART_Status_Success != uart_status) {
        abort();
    }

    return status;
}

BinaryLoggerStatus log_data(logger_size_t num_bytes, uint8_t *buffer)
{
    BinaryLoggerStatus status = BinaryLogger_Success;
    CircularBufferStatus cb_status;
    for (logger_size_t n = 0; n < num_bytes; n++) {
        bool is_full;
        do {
            // poll the transmit buffer to see if in can accept data.
            cb_status = CircularBufferIsFull(logger.transmit_buffer, &is_full);
        } while (is_full);
        cb_status = CircularBufferAddItem(logger.transmit_buffer, buffer + n);
        if (CircularBuffer_Success != cb_status) {
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
        cb_status = CircularBufferIsEmpty(logger.transmit_buffer, &is_empty);
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
    UartStatus uart_status = UART_Status_Success;
    CircularBufferStatus cb_status = CircularBuffer_Success;

#if (PLATFORM == PLATFORM_FRDM) && (LOGGER_ALGORITHM == LOGGER_INTERRUPTS)
    // extraction from uart receive data register and pack into the circular
    // buffer is done by the UART0 interrupt handler
    (void)uart_status; // keep the compiler happy
#else // LOGGER_ALGORITHM == LOGGER_POLLING
    for (logger_size_t n = 0; n < num_bytes; n++) {
        uint8_t byte;
        uart_status = logger.uart.receive_byte(&byte);
        if (UART_Status_Success != uart_status) {
            status = BinaryLogger_Error;
        }
        cb_status = CircularBufferAddItem(logger.receive_buffer, &byte);
        if (CircularBuffer_Success != cb_status) {
            abort();
        }
    }
#endif
    // extract from circular buffer and pack into user buffer.
    logger_size_t num_received = 0;
    while (num_received < num_bytes) {
        uint8_t byte;
        cb_status = CircularBufferRemoveItem(logger.receive_buffer, (void *)(&byte));
        if (CircularBuffer_Success == cb_status) {
            *(buffer + num_received) = byte;
            num_received++;
        }
        // FIXME(bja, 2017-03) error handling if we didn't get our desired number of bytes? just return what we got...
    }
    return status;
}

#else // LOGGING_DISABLED - disable logging by providing empty functions that
// simply return status OK. These may be removed by the linker when link
// time optimization in enabled. If profiling shows that they are not, we
// can replace them with a macro that substitutes status ok.
BinaryLoggerStatus BinaryLoggerInitialize(logger_size_t num_bytes)
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
