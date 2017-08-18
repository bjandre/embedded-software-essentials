
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

#include "communication_peripheral.h"
#include "uart_host.h"


CommStatus host_uart_initialize(const uint32_t baud)
{
    /* nothing to do */
    CommStatus status = Comm_Status_Success;
    return status;
}

CommStatus host_uart_transmit_byte(const uint8_t byte)
{
    CommStatus status = Comm_Status_Success;
    fprintf(stdout, "%.2x", byte);
    /*putc(byte, stdout); */
    return status;
}

CommStatus host_uart_receive_byte(uint8_t *byte)
{
    CommStatus status = Comm_Status_Success;
    int8_t value = fgetc(stdin);
    if (EOF == value) {
        status = Comm_Status_Error;
    } else {
        *byte = (uint8_t)value;
    }
    /* fgetc reads a single character. Terminal input will contain an extra */
    /* character for the newline. Need to remove it with another fgetc. */
//    if ('\n' == *byte) {
//        /* user input just a newline, do nothing */
//    } else {
//        fgetc(stdin);
//    }
    return status;
}
