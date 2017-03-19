
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

#include "MKL25Z4.h"

#include "circular_buffer.h"

#include "logger.h"

#include "async-global.h"

extern volatile BinaryLogger_t logger;
extern volatile async_data_t global_async_data;

void frdm_kl25z_initialize_interrupts(void)
{
    NVIC_EnableIRQ(UART0_IRQn);

}

extern void UART0_IRQHandler(void)
{
    CircularBufferStatus cb_status = CB_No_Error;
    uint8_t byte;
    // What triggered the interrupt...
    if (UART0->S1 & UART0_S1_RDRF_MASK) {
        // received data register full
        byte = UART0->D;
        {
            // NOTE(bja, 2017-03) critical region accessing global data.
            cb_status = CircularBufferAddItem(logger.receive_buffer, &byte);
            global_async_data.data_available = true;
        }
        if (CB_No_Error == cb_status) {
            // do nothing? status flag is automatically reset
        } else {
            // error handling?
        }
    } else if (UART0->S1 & UART0_S1_TDRE_MASK) {
        // transmit data register empty
        {
            // NOTE(bja, 2017-03) critical region accessing global data.
            cb_status = CircularBufferRemoveItem(logger.transmit_buffer, &byte);
        }
        if (CB_No_Error == cb_status) {
            // successfully removed item.
            UART0->D = byte;
        } else {
            // nothing else to send
            UART0->C2 &= ~UART0_C2_TIE(1);
        }
    } else {
        // other interrupts to handle?
    }
}
