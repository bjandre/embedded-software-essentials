
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef ESE_ARCH_UART_H_
#define ESE_ARCH_UART_H_

#include <stddef.h>
#include <stdint.h>

#include "uart-common.h"

typedef struct uart_t uart_t;

struct uart_t {
    UartStatus (*initialize)(const uint32_t baud);
    UartStatus (*transmit_byte)(const uint8_t byte);
    UartStatus (*transmit_n_bytes)(uart_t *uart, const size_t num_bytes,
                                   uint8_t *bytes);
    UartStatus (*receive_byte)(uint8_t *byte);
    UartStatus (*receive_n_bytes)(uart_t *uart, const size_t num_bytes,
                                  uint8_t *bytes);
};


/**
   CreateUART()

   Create a uart data strucure with pointers to the correct functions for th
   desired functionality, e.g. logger uart, and platform.

   Params: uart - pointer to the struct of function pointers.
           uart_function - the type of uart functionality

   Returns: status of the create operation

 */
UartStatus CreateUART(uart_t volatile *uart, UartFunction uart_func);



#endif // ESE_ARCH_UART_H_
