
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

/**
 * \file spi_frdm_kl25z.h
 *
 * SPI hardware specific implementation for FRDM-KL25Z dev board.
 */

#ifndef ESE_HAL_SPI_FRDM_KL25Z_H_
#define ESE_HAL_SPI_FRDM_KL25Z_H_

#include <stdbool.h>
#include <stdint.h>

#include "spi_peripheral.h"
#include "gpio_frdm_kl25z.h"

/**
   frdm-kl25z specific initialization for the spi. Conforms to the call
   signature of the initialization routine in the spi_peripheral_t
   defined in communication-peripheral.h

   \return status of the operation
 */
SPIStatus frdm_kl25z_spi_initialize(spi_peripheral_t volatile *this,
                                    const uint32_t speed);

/**
   frdm-kl25z specific shutdown for the spi. Conforms to the call
   signature of the shutdown routine in the spi_peripheral_t
   defined in communication-peripheral.h

   \return status of the operation
 */
SPIStatus frdm_kl25z_spi_shutdown(spi_peripheral_t volatile *this);

/**
   frdm-kl25z specific initialization for the spi transmit_byte. Conforms to the
   call signature of the transmit_byte routine in the spi_peripheral_t
   defined in communication-peripheral.h

   \param[in] byte - the byte to be sent

   \return status of the operation
 */
SPIStatus frdm_kl25z_spi_transmit_byte(spi_peripheral_t *this,
                                       const uint8_t byte);

/**
   frdm-kl25z specific initialization for the spi transmit_n_bytes. Conforms to the
   call signature of the transmit_byte routine in the spi_peripheral_t
   defined in communication-peripheral.h

   \param[in] byte - the byte to be sent

   \return status of the operation
 */
SPIStatus frdm_kl25z_spi_transmit_n_bytes(spi_peripheral_t *this,
        uint8_t const *const byte, const size_t num_bytes);

/**
   frdm-kl25z specific initialization for the spi receive_byte. Conforms to the
   call signature of the receive_byte routine in the spi_peripheral_t
   defined in communication-peripheral.h

   \param[out] byte - location to store the received byte.

   \return status of the operation
 */

SPIStatus frdm_kl25z_spi_receive_byte(spi_peripheral_t *this,
                                      uint8_t *byte);

/**
   frdm-kl25z specific initialization for the spi receive_n_byte. Conforms to the
   call signature of the receive_byte routine in the spi_peripheral_t
   defined in communication-peripheral.h

   \param[out] byte - location to store the received byte.

   \return status of the operation
 */
SPIStatus frdm_kl25z_spi_receive_n_bytes(spi_peripheral_t *this,
        uint8_t *bytes, const size_t num_bytes);

/**
   frdm-kl25z specific initialization for the spi
   polling_transmit_receive_byte. Conforms to the call signature of the
   spi_peripheral_t defined in
   spi-peripheral.h

   \param[in, out] *this - location spi specific state
   \param[in, out] *byte - pointer to the storage for the byte to send and receive

   \return status of the operation
 */

SPIStatus frdm_kl25z_spi_polling_transmit_receive_byte(
    spi_peripheral_t volatile *this, uint8_t *byte);

/**
   frdm-kl25z specific initialization for the spi
   polling_transmit_receive_n_bytes. Conforms to the call signature of the
   routine in spi_peripheral_t defined in spi-peripheral.h

   Note: send/receive data is handled through the spi peripheral transmit_buffer
   and receive_buffer.

   \param[in, out] *this - location spi specific state
   \param[in, out] *byte - pointer to the storage for the bytes to send and receive

   \return status of the operation
 */
SPIStatus frdm_kl25z_spi_polling_transmit_receive_n_bytes(
    spi_peripheral_t volatile *this, const size_t num_bytes);

/**
   Flush the communication peripheral transmit buffer.

   \param[in] *this - pointer to the communication peripheral

   \return status of the operation
*/
SPIStatus frdm_kl25z_spi_flush_transmit_buffer(spi_peripheral_t *this);

/**
   Pointer to the function to begin an asynchronous tranmit on the
   communication peripheral

*/
void frdm_kl25z_spi_begin_async_transmit(void);

#endif /* ESE_HAL_SPI_FRDM_KL25Z_H_ */
