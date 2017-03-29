
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

/**
   \file spi-frdm-kl25z.c

   Hardware specific SPI implementation for FRDM-KL25Z dev board
 */

#include <stddef.h>
#include <stdint.h>

#include "MKL25Z4.h"
#include "system_MKL25Z4.h"

#include "spi-common.h"
#include "spi-frdm-kl25z.h"

#include "circular_buffer.h"

SPIstatus frdm_kl25z_initialize_spi(void)
{
    /*

       SPI0 is clocked on the bus clock. SPI1 is clocked from the system
       clock. SPI1 is therefore disabled in "Partial Stop Mode".

       SPI0 is available on port C pins 4-7 and all pins are available on the
       header.

       Choose SPI1 active on port D because port D is also active for the blue
       LED and all pins are exposed on the dev board headers.

       KL25 family reference manual - Section 10.3.1 and FRDM-KL25Z valid pinout
       chart.

       PTD4 - Alternate function 2 - SPI1_PCS0 - slave select
       PTD5 - Alternate function 2 - SPI1_SCK - clock
       PTD6 - Alternate function 2 - SPI1_MOSI - Master Out Slave In
       PTD7 - Alternate function 2 - SPI1_MISO - Master In Slave Out

       SPI registers are 8 bit!

       Clock, polarity, phase, MSB are configured to talk to the Nordic nRF24L01
       chip.

    */

    // enable the clock for port d pins
    SPIstatus status = SPI_Status_Success;
    SIM->SCGC5 |= SIM_SCGC5_PORTD(1);
    // enable clock for SPI1 internal peripheral
    SIM->SCGC4 |= SIM_SCGC4_SPI1(1);

    // set the port d pins to alternate function 2
    PORTD->PCR[4] |= PORT_PCR_MUX(2);
    PORTD->PCR[5] |= PORT_PCR_MUX(2);
    PORTD->PCR[6] |= PORT_PCR_MUX(2);
    PORTD->PCR[7] |= PORT_PCR_MUX(2);

    // clear control registers
    SPI1->C1 = 0x0U;
    SPI1->C2 = 0x0U;

    // enable spi, SPE = 1
    SPI1->C1 |= SPI_C1_SPE(1);

    // configure as master, master=1, slave=0
    SPI1->C1 |= SPI_C1_MSTR(1);

    // clock polarity, active high / idle low when set to zero
    SPI1->C1 &= ~SPI_C1_CPOL(1);

    // clock phase, middle = 0, rising edge = 1
    SPI1->C1 &= ~SPI_C1_CPHA(1);

    // transfer MSB first = 0, LSB first = 1
    SPI1->C1 &= ~SPI_C1_LSBFE(1);

    // configrue for two wire, SPC0=0, (not one wire bidirectional, SPC0=1)
    SPI1->C2 &= ~SPI_C2_SPC0(1);

    // SPI1 uses system clock. We need a max of 8MHz
    // uint32_t system_clock = SystemCoreClock;

    // setup for the specified baud rate
    SPI1->BR = 0x0U;
    SPI1->BR |= SPI_BR_SPPR(2);
    //SPI1->BR |= SPI1_BR_SPR();


    // enable interrupts for the transmitter and receiver

    // enable the transmitter and receiver

    return status;
}

SPIstatus frdm_kl25z_spi_transmit_byte(const uint8_t byte)
{
    SPIstatus status = SPI_Status_Success;

    // poll the status register until empty. SPTEF == 1 --> empty
    bool transmit_buffer_empty = 0;
    while (!transmit_buffer_empty) {
        transmit_buffer_empty = SPI1->S & SPI_S_SPTEF(1);
    }

    // send a character
    SPI1->D = byte;

    return status;
}

SPIstatus frdm_kl25z_spi_transmit_n_bytes(const size_t num_bytes,
        uint8_t *bytes)
{
    SPIstatus status = SPI_Status_Success;
    for (size_t n = 0; n < num_bytes; n++) {
        frdm_kl25z_spi_transmit_byte(*(bytes + n));
    }
    return status;
}

SPIstatus frdm_kl25z_spi_receive_byte(uint8_t *byte)
{
    SPIstatus status = SPI_Status_Success;
    return status;
}

SPIstatus frdm_kl25z_spi_receive_n_bytes(const size_t num_bytes,
        uint8_t *bytes)
{
    SPIstatus status = SPI_Status_Success;
    for (size_t n = 0; n < num_bytes; n++) {
        frdm_kl25z_spi_receive_byte(bytes + n);
    }
    return status;
}
