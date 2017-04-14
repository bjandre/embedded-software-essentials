
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

#include "circular_buffer.h"

#include "spi-peripheral.h"
#include "spi-peripheral-factory.h"

#include "platform-defs.h"

#if (PLATFORM == PLATFORM_HOST) || (PLATFORM == PLATFORM_BBB)
#include "uart-host.h"
#elif (PLATFORM == PLATFORM_FRDM)
#include "spi-frdm-kl25z.h"
#endif

/**
   Setup the platform specific SPI for the nRF24L01+ SPI interface.

   \param[in,out] spi pointer to the spi data structure with platform specific
                  function pointers.

   \return status of the operation
 */
SPIStatus SetupSPInRF24(spi_peripheral_t volatile *spi);

/**
 * Clear pointers in an spi peripheral object
 *
 * \param[in,out] spi pointer to an spi peripheral
 */
void SPIClear(spi_peripheral_t volatile *spi);

SPIStatus SPICreate(spi_peripheral_t volatile *spi,
                    SPIFunction spi_function,
                    size_t num_bytes_buffer)
{
    SPIStatus status = SPI_Status_Success;
    if (NULL == spi) {
        status = SPI_Null_Pointer;
    } else {
        SPIClear(spi);

        switch (spi_function) {
        case SPI_nRF24:
            SetupSPInRF24(spi);
            break;
        default:
            // FIXME(bja, 2017-04) Additional spi peripheral functionality not
            // supported at this time.
            abort();
        }

        spi->initialize(spi, 0);

        const size_t bytes_per_item = 1;
        CircularBufferStatus cb_status = CircularBufferNew(
                                             &(spi->transmit_buffer),
                                             num_bytes_buffer,
                                             bytes_per_item);
        if (CircularBuffer_Success != cb_status) {
            status = SPI_Buffer_Error;
        }
        cb_status = CircularBufferNew(
                        &(spi->receive_buffer),
                        num_bytes_buffer,
                        bytes_per_item);
        if (CircularBuffer_Success != cb_status) {
            status = SPI_Buffer_Error;
        }
    }
    return status;
}

SPIStatus SetupSPInRF24(spi_peripheral_t volatile *spi)
{
    SPIStatus status = SPI_Status_Success;
#if (PLATFORM == PLATFORM_HOST) || (PLATFORM == PLATFORM_BBB)
    // FIXME(bja, 2017-04) generalize the uart interface to a generic host spi.
    abort();
#elif PLATFORM == PLATFORM_FRDM
    spi->initialize = &frdm_kl25z_spi_initialize;
    spi->transmit_byte = &frdm_kl25z_spi_transmit_byte;
    spi->receive_byte = &frdm_kl25z_spi_receive_byte;
    spi->begin_async_transmit = &frdm_kl25z_spi_begin_async_transmit;
    spi->flush_transmit_buffer = &frdm_kl25z_spi_flush_transmit_buffer;
    spi->transmit_n_bytes = &frdm_kl25z_spi_transmit_n_bytes;
    spi->receive_n_bytes = &frdm_kl25z_spi_receive_n_bytes;
    spi->polling_transmit_receive_byte =
        &frdm_kl25z_spi_polling_transmit_receive_byte;
    spi->polling_transmit_receive_n_bytes =
        &frdm_kl25z_spi_polling_transmit_receive_n_bytes;
    spi->begin_async_transmit = &frdm_kl25z_spi_begin_async_transmit;
    spi->flush_transmit_buffer = &frdm_kl25z_spi_flush_transmit_buffer;
#else
#error "ERROR: nRF24L01+ SPI interface not implemented for PLATFORM " PLATFORM
#endif
    return status;
}

void SPIClear(spi_peripheral_t volatile *spi)
{
    if (NULL != spi) {
        spi->initialize = NULL;
        spi->transmit_byte = NULL;
        spi->receive_byte = NULL;
        spi->begin_async_transmit = NULL;
        spi->flush_transmit_buffer = NULL;
        spi->transmit_n_bytes = NULL;
        spi->receive_n_bytes = NULL;
        spi->begin_async_transmit = NULL;
        spi->flush_transmit_buffer = NULL;
        spi->transmit_buffer = NULL;
        spi->receive_buffer = NULL;
    }
}

void SPIDestroy(spi_peripheral_t volatile *spi)
{
    if (NULL != spi) {
        if (NULL != spi->transmit_buffer) {
            CircularBufferDestroy(&(spi->transmit_buffer));
        }
        if (NULL != spi->receive_buffer) {
            CircularBufferDestroy(&(spi->receive_buffer));
        }
        SPIClear(spi);
    }
}
