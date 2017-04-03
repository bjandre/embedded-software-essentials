
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

#include "communication-peripheral.h"
#include "communication-peripheral-generic.h"

void comm_begin_async_transmit(void)
{
    /* do nothing to do for the generic case. Platform specific implementations
       will use interrupts or threads. */
}

CommStatus comm_transmit_n_bytes(communication_peripheral_t *comm,
                                 const size_t num_bytes, uint8_t *bytes)
{
    CommStatus status = Comm_Status_Success;
    if (NULL == comm) {
        status = Comm_Status_Error;
    } else {
        for (size_t n = 0; n < num_bytes; n++) {
            comm->transmit_byte(*(bytes + n));
        }
    }
    return status;
}

CommStatus comm_receive_n_bytes(communication_peripheral_t *comm,
                                const size_t num_bytes,
                                uint8_t *bytes)
{
    CommStatus status = Comm_Status_Success;
    if (NULL == comm) {
        status = Comm_Status_Error;
    } else {
        for (size_t n = 0; n < num_bytes; n++) {
            comm->receive_byte(bytes + n);
        }
    }
    return status;
}

CommStatus comm_flush_transmit_buffer(communication_peripheral_t *comm)
{
    CommStatus status = Comm_Status_Success;
    (void)comm;
    return status;
}

