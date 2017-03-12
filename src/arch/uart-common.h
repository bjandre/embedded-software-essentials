
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef ESE_ARCH_UART_COMMON_H_
#define ESE_ARCH_UART_COMMON_H_

#include <stdint.h>

#include "uart-common.h"

typedef enum UartStatus_t {
    UART_Status_OK,
    UART_Status_Error,
} UartStatus;

typedef enum UartFunction_t {
    UartDebugger,
    UartGeneral,
} UartFunction;

#endif // ESE_ARCH_UART_COMMON_H_
