
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

/**
   \file spi-bbb.c

   Hardware specific SPI implementation for Beagle Bone Black
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/ioctl.h>
#include <sys/stat.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#include "communication-peripheral.h"
#include "spi-bbb.h"

#include "circular_buffer.h"

/**
   spi state that must be preserved between transmit/receive calls
 */
typedef struct _bbb_spi_state {
    uint32_t file_descriptor; /*<! file descriptor for the open device */
} bbb_spi_state_t;

/**
   get the state of the spi interface that may need to be preserved across
   function calls.

   \param[out] *state current state of the spi pins.
 */
void bbb_get_spi_state(bbb_spi_state_t *state, const GPIO_PINS pin);

/**
   set the state of the spi interface that may need to be preserved across
   function calls.

   \param[in] *state new state of the spi pins.
 */
void bbb_set_spi_state(bbb_spi_state_t *state, const GPIO_PINS pin);

SPIStatus bbb_spi_initialize(spi_peripheral_t volatile *this,
                             const uint32_t baud)
{
    /*

       * P9\_17 - dt offset 0x15c - mode 0 - SPI0\_CS0 - chip select
       * P9\_18 - dt offset 0x158 - mode 0 - SPI0\_D1 - data out - MOSI 
       * P9\_21 - dt offset 0x154 - mode 0 - SPI0\_D0 - data in MISO
       * P9\_22 - dt offset 0x150 - mode 0 - SPI0\_SCLK - clock

       SPI registers are 8 bit!

       Clock, polarity, phase, MSB are configured to talk to the Nordic nRF24L01
       chip.

    */

    SPIStatus status = SPI_Status_Success;

    uint32_t mode = 0;
    uint8_t bits = 8;
    uint32_t speed = 1000000;

    static const char *device = "/dev/spidev1.0";

    bbb_spi_state_t *state = malloc(sizeof(bbb_spi_state_t));
    if (NULL == state) {
        status = SPI_Initialization_Error;
        return status;
    }

    state->file_descriptor = open(device, O_RDWR);
    if (state->file_descriptor < 0) {
        status = SPI_Initialization_Error;
        return status;
    }

    // mode |= SPI_CPHA;
    // mode |= SPI_CPOL;
    // mode |= SPI_LSB_FIRST;
    // mode |= SPI_CS_HIGH;
    // mode |= SPI_3WIRE;
    // mode |= SPI_NO_CS;

	int32_t ret = ioctl(state->file_descriptor, SPI_IOC_WR_MODE32, &mode);
    if (-1 == ret) {
        status = SPI_Initialization_Error;
        return status;
    }
    ret = ioctl(state->file_descriptor, SPI_IOC_WR_BITS_PER_WORD, &bits);
    if (-1 == ret) {
        status = SPI_Initialization_Error;
        return status;
    }
    ret = ioctl(state->file_descriptor, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    if (-1 == ret) {
        status = SPI_Initialization_Error;
        return status;
    }

    this->state = state;
    
    return status;
}

SPIStatus bbb_spi_shutdown(spi_peripheral_t volatile *this)
{
    SPIStatus status = SPI_Status_Success;
    bbb_spi_state_t *state = this->state;
    if (NULL == state) {
        status = SPI_Null_Pointer;
    } else {
        close(state->file_descriptor);
        free(state);
    }
    return status;
}

SPIStatus bbb_spi_transmit_byte(spi_peripheral_t *this,
                                const uint8_t byte)
{
    SPIStatus status = SPI_Status_Success;
    status = bbb_spi_transmit_n_bytes(this, &byte, 1);
    return status;
}

SPIStatus bbb_spi_transmit_n_bytes(spi_peripheral_t *this,
                                   uint8_t const *const byte, const size_t num_bytes)
{
    SPIStatus status = SPI_Status_Success;
    if (NULL == this) {
        status = SPI_Null_Pointer;
    }
    if (NULL == this->state) {
        status = SPI_Null_Pointer;
    }
    if (status != SPI_Status_Success) {
        return status;
    }

    struct spi_ioc_transfer transfer;
    transfer.tx_buf = (uint32_t)byte;
    transfer.len = num_bytes;
    
    bbb_spi_state_t *state = this->state;
	int32_t ret = ioctl(state->file_descriptor, SPI_IOC_MESSAGE(1), &transfer);
	if (ret < 1) {
		status = SPI_Status_Error;
    }

    return status;
}

SPIStatus bbb_spi_polling_transmit_receive_byte(
    spi_peripheral_t volatile *this, uint8_t *byte)
{
    SPIStatus status = SPI_Status_Success;
    if (NULL == this) {
        status = SPI_Null_Pointer;
    }
    if (NULL == this->state) {
        status = SPI_Null_Pointer;
    }
    if (status != SPI_Status_Success) {
        return status;
    }

    uint8_t rx_byte;
    uint32_t num_bytes = 1;
        
    struct spi_ioc_transfer transfer;
    transfer.tx_buf = (uint32_t)byte;
    transfer.rx_buf = (uint32_t)&rx_byte;
    transfer.len = num_bytes;
    
    bbb_spi_state_t *state = this->state;
	int32_t ret = ioctl(state->file_descriptor, SPI_IOC_MESSAGE(1), &transfer);
	if (ret < 1) {
		status = SPI_Status_Error;
    }
    *byte = rx_byte;
    return status;
}

SPIStatus bbb_spi_polling_transmit_receive_n_bytes(
    spi_peripheral_t volatile *this, const size_t num_bytes)
{
    SPIStatus status = SPI_Status_Success;
    // preserve the current state so we can restore it at the end.
    //bbb_spi_state_t state;
    //bbb_get_spi_state(&state, this->CS_pin);

    // set CS active low.

    uint8_t byte;
    for (size_t i = 0; i < num_bytes; i++) {
        CircularBufferRemoveItem(this->transmit_buffer, &byte);
        this->polling_transmit_receive_byte(this, &byte);
        CircularBufferAddItem(this->receive_buffer, &byte);
    }

    // restore original state of the chip select pin
    //bbb_set_spi_state(&state, this->CS_pin);

    return status;
}

SPIStatus bbb_spi_receive_byte(spi_peripheral_t *this, uint8_t *byte)
{
    SPIStatus status = SPI_Status_Success;
    status = bbb_spi_receive_n_bytes(this, byte, 1);
    return status;
}


SPIStatus bbb_spi_receive_n_bytes(spi_peripheral_t *this, uint8_t *byte,
        const size_t num_bytes)
{
    SPIStatus status = SPI_Status_Success;
    if (NULL == this) {
        status = SPI_Null_Pointer;
    }
    if (NULL == this->state) {
        status = SPI_Null_Pointer;
    }
    if (status != SPI_Status_Success) {
        return status;
    }
            
    struct spi_ioc_transfer transfer;
    transfer.rx_buf = (uint32_t)byte;
    transfer.len = num_bytes;
    
    bbb_spi_state_t *state = this->state;
	int32_t ret = ioctl(state->file_descriptor, SPI_IOC_MESSAGE(1), &transfer);
	if (ret < 1) {
		status = SPI_Status_Error;
    }
    return status;
}

SPIStatus bbb_spi_flush_transmit_buffer(spi_peripheral_t *this)
{
    SPIStatus status = SPI_Status_Success;
    status = SPI_Status_Not_Implemented;
    return status;
}

void bbb_spi_begin_async_transmit(void)
{
}

void bbb_get_spi_state(bbb_spi_state_t *state, const GPIO_PINS pin)
{
}

void bbb_set_spi_state(bbb_spi_state_t *state, const GPIO_PINS pin)
{
}

