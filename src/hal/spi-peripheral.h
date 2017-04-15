
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef ESE_HAL_SPI_PERIPHERAL_H_
#define ESE_HAL_SPI_PERIPHERAL_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "gpio-common.h"

#include "circular_buffer.h"

/**
   spi state that must be preserved between transmit/receive calls
 */
typedef struct _spi_state {
    bool chip_select; /*<! state of the SPI CS pin */
} spi_state_t;

/**
   Status of communication peripheral operation
 */
typedef enum _SPIStatus {
    SPI_Status_Success,
    SPI_Status_Error,
    SPI_Null_Pointer,
    SPI_Initialization_Error,
    SPI_Buffer_Error,
} SPIStatus;

typedef struct spi_peripheral_t spi_peripheral_t;

/**
   Class defining a communication peripheral with a specified functionality and
   platform specific implementations.
 */
struct spi_peripheral_t {
    /**
       Pointer to the function containing initialization for the communication
       peripheral.

       \param[in] *this - pointer to the communication peripheral
       \param[in] baud - required baud

       \return status of the operation
    */
    SPIStatus (*initialize)(spi_peripheral_t volatile *this, const uint32_t baud);

    /**
       Pointer to the function for transmitting a byte with the current
       communication peripheral.

       \param[in] *this - pointer to the communication peripheral
       \param[in] byte - byte to be sent

       \return status of the operation
    */
    SPIStatus (*transmit_byte)(spi_peripheral_t *this, const uint8_t byte);

    /**
       Pointer to the function for transmitting N bytes from the specified
       communication peripheral.

       \param[in] *this - pointer to the communication peripheral
       \param[in] num_bytes - number of bytes to transmit
       \param[in] *byte - pointer to location to of bytes to transmit

       \return status of the operation
    */
    SPIStatus (*transmit_n_bytes)(spi_peripheral_t *this,
                                  uint8_t const *const bytes,
                                  const size_t num_bytes);

    /**
       Pointer to the function to begin an asynchronous tranmit on the
       communication peripheral

    */
    void (*begin_async_transmit)(void);

    /**
       Pointer to the function for receiving a byte from the specified
       communication peripheral.

       \param[in] *this - pointer to the communication peripheral
       \param[out] *byte - pointer to location to store the received byte

       \return status of the operation
    */
    SPIStatus (*receive_byte)(spi_peripheral_t *this, uint8_t *byte);

    /**
       Pointer to the function for receiving N bytes from the specified
       communication peripheral.

       \param[in] *this - pointer to the communication peripheral
       \param[in] num_bytes - number of bytes to receive
       \param[out] *byte - pointer to location to store the received bytes

       \return status of the operation
    */
    SPIStatus (*receive_n_bytes)(spi_peripheral_t *this,
                                 uint8_t *bytes,
                                 const size_t num_bytes);

    /**
       Pointer to the function for polling to transmit and receiving a single
       byte from the specified communication peripheral.

       \param[in] *this - pointer to the communication peripheral
       \param[in, out] *byte - pointer to location to store the received byte

       \return status of the operation
    */
    SPIStatus (*polling_transmit_receive_byte)(spi_peripheral_t volatile *this,
            uint8_t *byte);

    /**
       Pointer to the function for polling to transmit and receive N bytes from
       the specified communication peripheral.

       Note: send and receive data are handled through the spi peripheral
       transmit_buffer and receive_buffer.

       \param[in] *this - pointer to the communication peripheral
       \param[in] num_bytes - number of bytes to transmit and receive

       \return status of the operation
    */
    SPIStatus (*polling_transmit_receive_n_bytes)(spi_peripheral_t volatile *this,
            const size_t num_bytes);

    /**
       Flush the communication peripheral transmit buffer.

       \param[in] *this - pointer to the communication peripheral

       \return status of the operation
    */
    SPIStatus (*flush_transmit_buffer)(spi_peripheral_t *this);

    /**
       Internal state that needs to be preserved between calls
     */
    spi_state_t state;

    /**
       manually controled chip select pin
     */
    GPIO_PINS CS_pin;

    /**
       transmit buffer
     */
    CircularBuffer_t volatile *transmit_buffer;

    /**
       receive buffer
     */
    CircularBuffer_t volatile *receive_buffer;

};


#endif // ESE_HAL_SPI_PERIPHERAL_H_
