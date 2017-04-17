
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef ESE_HAL_HOST_UART_H_
#define ESE_HAL_HOST_UART_H_

#include <stdint.h>

#include "communication-peripheral.h"

/**
   Host specific initialization for the uart logger. Conforms to the call
   signature of the initialization routine for a communication_peripheral_t
   defined in communication-peripheral.h

   \param[in] baud - required baud

   \return status of the operation
 */
CommStatus host_uart_initialize(const uint32_t baud);

/**
   Host specific initialization for the uart_transmit_byte. Conforms to the call
   signature of the transmit_byte routine in the communication_peripheral_t
   defined in communication-peripheral.h

   \param[in] byte - the byte to be sent

   \return status of the operation
 */
CommStatus host_uart_transmit_byte(const uint8_t byte);

/**
   Host specific initialization for the uart_receive_byte. Conforms to the call
   signature of the receive_byte routine in the communication_peripheral_t
   defined in communication-peripheral.h

   \param[out] byte - location to store the received byte.

   \return status of the operation
 */
CommStatus host_uart_receive_byte(uint8_t *byte);



#endif/* ESE_HAL_HOST_UART_H_ */
