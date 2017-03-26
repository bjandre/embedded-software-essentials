
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

#ifndef ESE_ARCH_SPI_FRDM_KL25Z_H_
#define ESE_ARCH_SPI_FRDM_KL25Z_H_

#include <stdint.h>

#include "spi-common.h"

/**
   frdm-kl25z specific initialization for the spi. Conforms to the call
   signature of the initialization routine in the spi_t defined in spi.h

   \return status of the operation
 */
SPIstatus frdm_kl25z_initialize_spi(void);

/**
   frdm-kl25z specific initialization for the spi_transmit_byte. Conforms to
   the call signature of the transmit_byte routine in the spi_t defined in
   spi.h

   \param[in] byte - the byte to be sent

   \return status of the operation
 */
SPIstatus frdm_kl25z_spi_transmit_byte(const uint8_t byte);

/**
   frdm-kl25z specific initialization for the spi_receive_byte. Conforms to the
   call signature of the receive_byte routine in the spi_t defined in spi.h

   \param[out] byte - location to store the received byte.

   \return status of the operation
 */
SPIstatus frdm_kl25z_spi_receive_byte(uint8_t *byte);

#endif /* ESE_ARCH_SPI_FRDM_KL25Z_H_ */
