
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef ESE_HAL_UART_H_
#define ESE_HAL_UART_H_

#include <stddef.h>
#include <stdint.h>

#include "uart-common.h"

typedef struct uart_t uart_t;

/**
   Class defining a UART with a specified functionality and platform
 */
struct uart_t {
    /**
       Pointer to the function containing initialization for the UART.

       \param[in] baud - required baud

       \return status of the operation
    */
    UartStatus (*initialize)(const uint32_t baud);

    /**
       Pointer to the function for transmitting a byte with the current UART.

       \param[in] byte - byte to be sent

       \return status of the operation
    */
    UartStatus (*transmit_byte)(const uint8_t byte);

    /**
       Pointer to the function for transmitting N bytes from the specified UART.

       \param[in] *uart - pointer to the UART
       \param[in] num_bytes - number of bytes to transmit
       \param[in] *byte - pointer to location to of bytes to transmit

       \return status of the operation
    */
    UartStatus (*transmit_n_bytes)(uart_t *uart, const size_t num_bytes,
                                   uint8_t *bytes);

    /**
       Pointer to the function to begin an asynchronous tranmit on the uart

    */
    void (*begin_async_transmit)(void);

    /**
       Pointer to the function for receiving a byte from the specified UART.

       \param[out] *byte - pointer to location to store the received byte

       \return status of the operation
    */
    UartStatus (*receive_byte)(uint8_t *byte);

    /**
       Pointer to the function for receiving N bytes from the specified UART.

       \param[in] *uart - pointer to the UART
       \param[in] num_bytes - number of bytes to receive
       \param[out] *byte - pointer to location to store the received bytes

       \return status of the operation
    */
    UartStatus (*receive_n_bytes)(uart_t *uart, const size_t num_bytes,
                                  uint8_t *bytes);
};


/**
   Create a uart data strucure with pointers to the correct functions for the
   desired functionality, e.g. logger uart, and platform.

   Params: uart - pointer to the struct of function pointers.
           uart_function - the type of uart functionality

   Returns: status of the create operation

 */
UartStatus CreateUART(uart_t volatile *uart, UartFunction uart_func);


/**
   Destroy a UART object.
 */
void UARTdestroy(uart_t volatile *uart);

#endif // ESE_HAL_UART_H_
