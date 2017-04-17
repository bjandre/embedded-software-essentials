
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef ESE_HAL_SPI_PERIPHERAL_FACTORY_H_
#define ESE_HAL_SPI_PERIPHERAL_FACTORY_H_

#include "spi-peripheral.h"

/**
   Type of spi peripheral functionality
 */
typedef enum _SPIFunction {
    SPI_nRF24,
} SPIFunction;

/**
   \file spi-peripheral-factory.h

   Interface to create an instance of a spi peripheral
 */

/**
   Create a spi peripheral data strucure with pointers to the correct
   functions for the desired functionality, e.g. logger uart, spi peripheral.

   \param[out] spi pointer to the struct of function pointers.
   \param[in] spi_function the type of spi functionality

   \return status of the create operation
 */
SPIStatus SPICreate(spi_peripheral_t volatile *spi,
                    SPIFunction spi_function,
                    size_t num_bytes_buffer);


/**
   Destroy a spi peripheral object.

   \param[in] spi pointer to the struct of function pointers.

   \return status of the create operation
 */
void SPIDestroy(spi_peripheral_t volatile *spi);

#endif/* ESE_HAL_SPI_PERIPHERAL_FACTORY_H_ */
