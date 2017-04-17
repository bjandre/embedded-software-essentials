
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include <stddef.h>
#include <stdlib.h>

#include "communication-peripheral.h"
#include "communication-peripheral-generic.h"
#include "communication-peripheral-factory.h"

#include "platform-defs.h"

#if (PLATFORM == PLATFORM_HOST) || (PLATFORM == PLATFORM_BBB)
#include "uart-host.h"
#elif (PLATFORM == PLATFORM_FRDM)
#include "uart-frdm-kl25z.h"
#endif

/**
   Setup the platform specific COMM for the debugging logger.

   \param comm pointer to the comm data structure with platform specific
                  function pointers.

   \return status of the operation
 */
CommStatus SetupUARTLogger(communication_peripheral_t volatile *comm);


CommStatus CommCreate(communication_peripheral_t volatile *comm,
                      CommFunction comm_function)
{
    CommStatus status = Comm_Status_Success;
    if (NULL == comm) {
        status = Comm_Status_Error;
    } else {
        comm->transmit_n_bytes = &comm_transmit_n_bytes;
        comm->receive_n_bytes = &comm_receive_n_bytes;
        comm->begin_async_transmit = &comm_begin_async_transmit;
        comm->flush_transmit_buffer = &comm_flush_transmit_buffer;
        if (UART_Logger == comm_function) {
            SetupUARTLogger(comm);
        } else {
            /* FIXME(bja, 2017-03) Additional comm peripheral functionality not supported at */
            /* this time. */
            abort();
        }
    }
    return status;
}

CommStatus SetupUARTLogger(communication_peripheral_t volatile *comm)
{
    CommStatus status = Comm_Status_Success;
#if (PLATFORM == PLATFORM_HOST) || (PLATFORM == PLATFORM_BBB)
    comm->initialize = &host_uart_initialize;
    comm->transmit_byte = &host_uart_transmit_byte;
    comm->receive_byte = &host_uart_receive_byte;
#elif PLATFORM == PLATFORM_FRDM
    comm->initialize = &frdm_kl25z_uart_initialize;
    comm->transmit_byte = &frdm_kl25z_uart_transmit_byte;
    comm->receive_byte = &frdm_kl25z_uart_receive_byte;
    comm->begin_async_transmit = &frdm_kl25z_uart_begin_async_transmit;
    comm->flush_transmit_buffer = &frdm_kl25z_uart_flush_transmit_buffer;
#else
#error "ERROR: UART not implemented for unknown PLATFORM " PLATFORM
#endif
    return status;
}

void CommDestroy(communication_peripheral_t volatile *comm)
{
    if (NULL != comm) {
        comm->initialize = NULL;
        comm->transmit_byte = NULL;
        comm->receive_byte = NULL;
        comm->transmit_n_bytes = NULL;
        comm->receive_n_bytes = NULL;
        comm->begin_async_transmit = NULL;
    }
}
