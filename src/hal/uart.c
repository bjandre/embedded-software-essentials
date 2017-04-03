
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "uart.h"
#include "platform-defs.h"

#if (PLATFORM == PLATFORM_HOST) || (PLATFORM == PLATFORM_BBB)
#include "uart-host.h"
#elif (PLATFORM == PLATFORM_FRDM)
#include "uart-frdm-kl25z.h"
#endif

/**
   Platform independent routine to loop over n bytes and transmit them.

   Params: uart - pointer to the uart data structure with platform specific
                  function pointers.
           num_bytes - the number of bytes to transmit
           bytes - pointer to the buffer containing the data

   Returns: status of the operation
 */
UartStatus uart_transmit_n_bytes(uart_t *uart,
                                 const size_t num_bytes, uint8_t *bytes);


/**
   Platform independent routine to stort n bytes from the uart receiver

   Params: uart - pointer to the uart data structure with platform specific
                  function pointers.
           num_bytes - the number of bytes to receive
           bytes - pointer to the buffer where the data should be stored

   Returns: status of the operation
 */
UartStatus uart_receive_n_bytes(uart_t *uart,
                                const size_t num_bytes, uint8_t *bytes);


/**
   Setup the platform specific UART for the debugging logger.

   Params: uart - pointer to the uart data structure with platform specific
                  function pointers.

   Returns: status of the operation
 */
UartStatus uartSetupDebugger(uart_t volatile *uart);


/**
   Generic function to begin an asynchronous tranmit on uart. Does nothing for
   the case when interrupts are not available.
 */
void uart_begin_async_transmit(void)
{
    /* do nothing to do for the generic case. Platform specific implementations
       will use interrupts or threads. */
}

UartStatus uart_transmit_n_bytes(uart_t *uart,
                                 const size_t num_bytes, uint8_t *bytes)
{
    UartStatus status = UART_Status_Success;
    if (NULL == uart) {
        status = UART_Status_Error;
    } else {
        for (size_t n = 0; n < num_bytes; n++) {
            uart->transmit_byte(*(bytes + n));
        }
    }
    return status;
}

UartStatus uart_receive_n_bytes(uart_t *uart, const size_t num_bytes,
                                uint8_t *bytes)
{
    UartStatus status = UART_Status_Success;
    if (NULL == uart) {
        status = UART_Status_Error;
    } else {
        for (size_t n = 0; n < num_bytes; n++) {
            uart->receive_byte(bytes + n);
        }
    }
    return status;
}


UartStatus CreateUART(uart_t volatile *uart, UartFunction uart_function)
{
    UartStatus status = UART_Status_Success;
    if (NULL == uart) {
        status = UART_Status_Error;
    } else {
        uart->transmit_n_bytes = &uart_transmit_n_bytes;
        uart->receive_n_bytes = &uart_receive_n_bytes;
        uart->begin_async_transmit = &uart_begin_async_transmit;
        if (UartDebugger == uart_function) {
            uartSetupDebugger(uart);
        } else {
            // FIXME(bja, 2017-03) Additional UART functionality not supported at
            // this time.
            abort();
        }
    }
    return status;
}

UartStatus uartSetupDebugger(uart_t volatile *uart)
{
    UartStatus status = UART_Status_Success;
#if (PLATFORM == PLATFORM_HOST) || (PLATFORM == PLATFORM_BBB)
    uart->initialize = &host_uart_initialize;
    uart->transmit_byte = &host_uart_transmit_byte;
    uart->receive_byte = &host_uart_receive_byte;
#elif PLATFORM == PLATFORM_FRDM
    uart->initialize = &frdm_kl25z_uart_initialize;
    uart->transmit_byte = &frdm_kl25z_uart_transmit_byte;
    uart->receive_byte = &frdm_kl25z_uart_receive_byte;
    uart->begin_async_transmit = &frdm_kl25z_uart_begin_async_transmit;
#else
#error "ERROR: UART not implemented for unknown PLATFORM " PLATFORM
#endif
    return status;
}

void UARTdestroy(uart_t volatile *uart)
{
    if (NULL != uart) {
        uart->initialize = NULL;
        uart->transmit_byte = NULL;
        uart->receive_byte = NULL;
        uart->transmit_n_bytes = NULL;
        uart->receive_n_bytes = NULL;
        uart->begin_async_transmit = NULL;
    }
}
