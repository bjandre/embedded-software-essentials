
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
#include "binary_logger.h"
#include "async_global.h"

/**
   logger transmit using polling
 */
void logger_polling_transmit_byte(void);

extern volatile async_data_t global_async_data;

static const uint32_t debugger_baud = 115200u;/*!< UART debugger baud */
static const logger_size_t bytes_per_item = sizeof(
            uint8_t);/*!< number of bytes per sent via logger */

binary_logger_status_t binary_logger_create(logger_size_t num_bytes)
{
    binary_logger_status_t status = BINARY_LOGGER_SUCCESS;
    circular_buffer_status_t cb_status;
    logger_size_t num_buffer_items = num_bytes / bytes_per_item;

    binary_logger_t *logger = malloc(sizeof(binary_logger_t));
    cb_status = circular_buffer_new(&(logger->transmit_buffer),
                                    num_buffer_items,
                                    bytes_per_item);
    if (CIRCULAR_BUFFER_SUCCESS != cb_status) {
        return BINARY_LOGGER_ERROR;
    }
    cb_status = circular_buffer_new(&(logger->receive_buffer),
                                    num_buffer_items,
                                    bytes_per_item);
    if (CIRCULAR_BUFFER_SUCCESS != cb_status) {
        return BINARY_LOGGER_ERROR;
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

void binary_logger_destroy(void)
{
    {
        uint32_t interrupt_state = start_critical_region();
        binary_logger_t *logger = global_async_data.logger;
        if (NULL != logger) {
            circular_buffer_destroy(&(logger->transmit_buffer));
            circular_buffer_destroy(&(logger->receive_buffer));
            CommDestroy(&(logger->comm));
        }
        end_critical_region(interrupt_state);
    }
}

binary_logger_status_t binary_logger_log_data(logger_size_t num_bytes,
        uint8_t *buffer)
{
    binary_logger_status_t status = BINARY_LOGGER_SUCCESS;
    circular_buffer_status_t cb_status;
    uint32_t interrupt_state;
    for (logger_size_t n = 0; n < num_bytes; n++) {
        bool is_full = false;
        do {
            /* poll the transmit buffer to see if in can accept data. */
            {
                interrupt_state = start_critical_region();
                cb_status = circular_buffer_is_full(global_async_data.logger->transmit_buffer,
                                                    &is_full);
                end_critical_region(interrupt_state);
                if (CIRCULAR_BUFFER_SUCCESS != cb_status) {
                    abort();
                }

            }
        } while (is_full);
        {
            interrupt_state = start_critical_region();
            cb_status = circular_buffer_add_item(global_async_data.logger->transmit_buffer,
                                                 buffer + n);
            end_critical_region(interrupt_state);
        }
        if (CIRCULAR_BUFFER_SUCCESS != cb_status) {
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
    circular_buffer_status_t cb_status;
    bool is_empty = false;
    uint32_t interrupt_state;
    {
        interrupt_state = start_critical_region();
        cb_status = circular_buffer_is_empty(global_async_data.logger->transmit_buffer,
                                             &is_empty);
        end_critical_region(interrupt_state);
    }
    if (CIRCULAR_BUFFER_SUCCESS != cb_status) {
        abort();
    }
    while (!is_empty) {
        uint8_t byte;
        {
            interrupt_state = start_critical_region();
            cb_status = circular_buffer_remove_item(
                            global_async_data.logger->transmit_buffer,
                            &byte);
            if (CIRCULAR_BUFFER_SUCCESS == cb_status) {
                global_async_data.logger->comm.transmit_byte(byte);
            }

            circular_buffer_is_empty(global_async_data.logger->transmit_buffer, &is_empty);
            end_critical_region(interrupt_state);
        }
    }
}
binary_logger_status_t binary_logger_log_string(uint8_t *string)
{
    binary_logger_status_t status = BINARY_LOGGER_SUCCESS;
    size_t string_length = 0;
    while (*(string + string_length) != '\0') {
        string_length++;
    }
    binary_logger_log_data(string_length, (uint8_t *)string);
    return status;
}

binary_logger_status_t binary_logger_log_integer(int32_t integer)
{
    binary_logger_status_t status = BINARY_LOGGER_SUCCESS;
    const size_t max_digits = 32;
    char string[max_digits];
    const uint8_t null_char = '\0';
    memset_cpu((uint8_t *)string, &null_char, max_digits);
    uint32_t decimal_base = 10;
    my_itoa((int8_t *)string, integer, decimal_base);
    binary_logger_log_string((uint8_t *)string);
    return status;
}

binary_logger_status_t binary_logger_flush(void)
{
    binary_logger_status_t status = BINARY_LOGGER_SUCCESS;
    circular_buffer_status_t cb_status;

    bool is_empty = false;
    do {
        {
            uint32_t interrupt_state = start_critical_region();
            cb_status = circular_buffer_is_empty(global_async_data.logger->transmit_buffer,
                                                 &is_empty);
            end_critical_region(interrupt_state);
        }
        if (CIRCULAR_BUFFER_SUCCESS != cb_status) {
            status = BINARY_LOGGER_ERROR;
            break;
        }
    } while (!is_empty);
    return status;
}

binary_logger_status_t binary_logger_receive_data(logger_size_t num_bytes,
        uint8_t *buffer)
{
    binary_logger_status_t status = BINARY_LOGGER_SUCCESS;
    circular_buffer_status_t cb_status = CIRCULAR_BUFFER_SUCCESS;

#if (PLATFORM == PLATFORM_FRDM) && (LOGGER_ALGORITHM == LOGGER_INTERRUPTS)
    /* extraction from comm receive data register and pack into the circular */
    /* buffer is done by the communication peripheral interrupt handler */
#else/* LOGGER_ALGORITHM == LOGGER_POLLING */
    status = binary_logger_polling_receive(num_bytes);
#endif
    /* extract from circular buffer and pack into user buffer. */
    logger_size_t num_received = 0;
    while (num_received < num_bytes) {
        uint8_t byte;
        {
            uint32_t interrupt_state = start_critical_region();
            cb_status = circular_buffer_remove_item(
                            global_async_data.logger->receive_buffer,
                            (void *)(&byte));
            end_critical_region(interrupt_state);
        }
        if (CIRCULAR_BUFFER_SUCCESS == cb_status) {
            *(buffer + num_received) = byte;
            num_received++;
        } else { /* (CIRCULAR_BUFFER_SUCCESS != cb_status) */
            abort();
        }
        /* FIXME(bja, 2017-03) error handling if we didn't get our desired number of bytes? just return what we got... */
    }
    return status;
}

binary_logger_status_t binary_logger_polling_receive(logger_size_t num_bytes)
{
    binary_logger_status_t status = BINARY_LOGGER_SUCCESS;
    circular_buffer_status_t cb_status = CIRCULAR_BUFFER_SUCCESS;
    for (logger_size_t n = 0; n < num_bytes; n++) {
        uint8_t byte;
        {
            uint32_t interrupt_state = start_critical_region();
            CommStatus comm_status = global_async_data.logger->comm.receive_byte(&byte);
            if (Comm_Status_Success != comm_status) {
                status = BINARY_LOGGER_ERROR;
            } else {
                cb_status = circular_buffer_add_item(global_async_data.logger->receive_buffer,
                                                     &byte);
            }
            end_critical_region(interrupt_state);
        }
        if (CIRCULAR_BUFFER_SUCCESS != cb_status) {
            abort();
        }
    }
    return status;
}

#else/* LOGGING_DISABLED - disable logging by providing empty functions that */
// simply return status OK. These may be removed by the linker when link
// time optimization in enabled. If profiling shows that they are not, we
// can replace them with a macro that substitutes status ok.
binary_logger_status_t binary_logger_create(logger_size_t num_bytes)
{
    (void)num_bytes;
    return BINARY_LOGGER_SUCCESS;
}

binary_logger_status_t binary_logger_log_data(logger_size_t num_bytes,
        uint8_t *buffer)
{
    (void)num_bytes;
    (void)buffer;
    return BINARY_LOGGER_SUCCESS;
}

binary_logger_status_t binary_logger_log_string(uint8_t *string)
{
    (void)string;
    return BINARY_LOGGER_SUCCESS;
}

binary_logger_status_t binary_logger_log_integer(int32_t integer)
{
    (void)integer;
    return BINARY_LOGGER_SUCCESS;
}

binary_logger_status_t binary_logger_flush(void)
{
    return BINARY_LOGGER_SUCCESS;
}

binary_logger_status_t binary_logger_receive_data(logger_size_t num_bytes,
        uint8_t *buffer)
{
    (void)num_bytes;
    (void)buffer;
    return BINARY_LOGGER_SUCCESS;
}
#endif
