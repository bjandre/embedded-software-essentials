
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef ESE_HAL_COMMUNICATION_PERIPHERAL_GENERIC_H_
#define ESE_HAL_COMMUNICATION_PERIPHERAL_GENERIC_H_

#include <stdint.h>

#include "communication-peripheral.h"

/**
   \file communication-peripheral-generic.h

   Platform independent implementations of communication peripheral routines.
 */

/**
   Platform independent routine to loop over n bytes and transmit them.

   \param[in] comm pointer to the comm data structure with platform specific
                  function pointers.
   \param[in] num_bytes the number of bytes to transmit
   \param[in] bytes pointer to the buffer containing the data

   \return status of the operation
 */
CommStatus comm_transmit_n_bytes(communication_peripheral_t *comm,
                                 const size_t num_bytes, uint8_t *bytes);


/**
   Platform independent routine to stort n bytes from the comm receiver

   \param[in] comm pointer to the comm data structure with platform specific
                  function pointers.
   \param[in] num_bytes the number of bytes to receive
   \param[out] bytes pointer to the buffer where the data should be stored

   \return status of the operation
 */
CommStatus comm_receive_n_bytes(communication_peripheral_t *comm,
                                const size_t num_bytes, uint8_t *bytes);


/**
   Platform independent to routine to flush the transmit buffer.

   \param[in] comm pointer to the comm data structure with platform specific
                  function pointers.

   \return status of the operation
 */
CommStatus comm_flush_transmit_buffer(communication_peripheral_t *comm);

/**
   Generic function to begin an asynchronous tranmit on comm. Does nothing for
   the case when interrupts are not available.
 */
void comm_begin_async_transmit(void);

#endif // ESE_HAL_COMMUNICATION_PERIPHERAL_GENERIC_H_
