
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef ESE_HAL_UART_COMMON_H_
#define ESE_HAL_UART_COMMON_H_

#include <stdint.h>

/**
   Status of UART operation
 */
typedef enum UartStatus {
    UART_Status_Success,
    UART_Status_Error,
} UartStatus;

/**
   Type of UART functionality
 */
typedef enum UartFunction_t {
    UartDebugger,
    UartGeneral,
} UartFunction;

#endif // ESE_HAL_UART_COMMON_H_
