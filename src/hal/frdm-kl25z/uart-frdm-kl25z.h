
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

/*
 * frdm-kl25z-uart.h
 *
 *  Created on: Mar 3, 2017
 *      Author: andreb
 */

#ifndef ESE_HAL_UART_FRDM_KL25Z_H_
#define ESE_HAL_UART_FRDM_KL25Z_H_

#include <stdint.h>

#include "uart-common.h"

/**
   frdm-kl25z specific initialization for the uart. Conforms to the call
   signature of the initialization routine in the uart_t defined in uart.h

   \param[in] baud - required baud

   \return status of the operation
 */
UartStatus frdm_kl25z_uart_initialize(const uint32_t baud);

/**
   frdm-kl25z specific initialization for the uart_transmit_byte. Conforms to
   the call signature of the transmit_byte routine in the uart_t defined in
   uart.h

   \param[in] byte - the byte to be sent

   \return status of the operation
 */
UartStatus frdm_kl25z_uart_transmit_byte(const uint8_t byte);

/**
   frdm-kl25z specific initialization for the uart_receive_byte. Conforms to the
   call signature of the receive_byte routine in the uart_t defined in uart.h

   \param[out] byte - location to store the received byte.

   \return status of the operation
 */
UartStatus frdm_kl25z_uart_receive_byte(uint8_t *byte);

#endif /* ESE_HAL_UART_FRDM_KL25Z_H_ */
