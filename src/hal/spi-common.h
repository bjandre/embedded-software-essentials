
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef ESE_HAL_SPI_COMMON_H_
#define ESE_HAL_SPI_COMMON_H_

#include <stdint.h>

/**
   \file spi-common.h

   Common infrastructure used by all SPI implementations
 */

/**
   Status of SPI operation
 */
typedef enum SPIstatus {
    SPI_Status_Success,
    SPI_Status_Error,
    SPI_Status_NULL,
} SPIstatus;


#endif/* ESE_HAL_SPI_COMMON_H_ */
