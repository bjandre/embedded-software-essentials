
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

/**
 * \file spi-frdm-kl25z.h
 *
 * SPI hardware specific implementation for FRDM-KL25Z dev board.
 */

#ifndef ESE_HAL_SPI_FRDM_KL25Z_H_
#define ESE_HAL_SPI_FRDM_KL25Z_H_

#include <stdint.h>

#include "communication-peripheral.h"

/**
   frdm-kl25z specific initialization for the spi. Conforms to the call
   signature of the initialization routine in the communication_peripheral_t
   defined in communication-peripheral.h

   \return status of the operation
 */
CommStatus frdm_kl25z_initialize_spi(void);

/**
   frdm-kl25z specific initialization for the spi transmit_byte. Conforms to the
   call signature of the transmit_byte routine in the communication_peripheral_t
   defined in communication-peripheral.h

   \param[in] byte - the byte to be sent

   \return status of the operation
 */
CommStatus frdm_kl25z_spi_transmit_byte(const uint8_t byte, const GPIO_PINS pin);

/**
   frdm-kl25z specific initialization for the spi receive_byte. Conforms to the
   call signature of the receive_byte routine in the communication_peripheral_t
   defined in communication-peripheral.h

   \param[out] byte - location to store the received byte.

   \return status of the operation
 */
CommStatus frdm_kl25z_spi_receive_byte(uint8_t *byte);

#endif /* ESE_HAL_SPI_FRDM_KL25Z_H_ */
