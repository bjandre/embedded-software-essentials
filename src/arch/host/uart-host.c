
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include <stdint.h>
#include <stdio.h>

#include "uart-common.h"
#include "uart-host.h"


UartStatus host_uart_initialize(const uint32_t baud)
{
    // nothing to do
    UartStatus status = UART_Status_Success;
    return status;
}

UartStatus host_uart_transmit_byte(const uint8_t byte)
{
    UartStatus status = UART_Status_Success;
    fprintf(stdout, "%.2x", byte);
    //putc(byte, stdout);
    return status;
}

UartStatus host_uart_receive_byte(uint8_t *byte)
{
    UartStatus status = UART_Status_Success;
    *byte = fgetc(stdin);
    // fgetc reads a single character. Terminal input will contain an extra
    // character for the newline. Need to remove it with another fgetc.
    if ('\n' == *byte) {
        // user input just a newline, do nothing
    } else {
        fgetc(stdin);
    }
    return status;
}
