
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

/**
   \file spi_frdm_kl25z.c

   Hardware specific SPI implementation for FRDM-KL25Z dev board
 */

#include <stdbool.h>
#include <stdint.h>

#include "MKL25Z4.h"
#include "system_MKL25Z4.h"

#include "gpio_frdm_kl25z.h"
#include "communication_peripheral.h"
#include "spi_frdm_kl25z.h"

#include "circular_buffer.h"

/**
   spi state that must be preserved between transmit/receive calls
 */
typedef struct _frdm_kl25z_spi_state {
    bool chip_select; /*<! state of the SPI CS pin */
} frdm_kl25z_spi_state_t;

/**
   get the state of the spi interface that may need to be preserved across
   function calls.

   \param[out] *state current state of the spi pins.
 */
void frdm_kl25z_get_spi_state(frdm_kl25z_spi_state_t *state, const GPIO_PINS pin);

/**
   set the state of the spi interface that may need to be preserved across
   function calls.

   \param[in] *state new state of the spi pins.
 */
void frdm_kl25z_set_spi_state(frdm_kl25z_spi_state_t *state, const GPIO_PINS pin);


SPIStatus frdm_kl25z_spi_initialize(spi_peripheral_t volatile *this,
                                    const uint32_t baud)
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

       PTD3 - Alternate function 1 - GPIO - device enable, manually controlled
       PTD4 - Alternate function 1 - GPIO - slave select, manually controlled
       PTD5 - Alternate function 2 - SPI1_SCK - clock
       PTD6 - Alternate function 2 - SPI1_MOSI - Master Out Slave In
       PTD7 - Alternate function 2 - SPI1_MISO - Master In Slave Out

       SPI registers are 8 bit!

       Clock, polarity, phase, MSB are configured to talk to the Nordic nRF24L01
       chip.

    */

    // enable the clock for port d pins
    SPIStatus status = SPI_Status_Success;
    SIM->SCGC5 |= SIM_SCGC5_PORTD(1);
    // enable clock for SPI1 internal peripheral
    SIM->SCGC4 |= SIM_SCGC4_SPI1(1);

    // set the port d pins to alternate function 1, GPIO, for manually controlled pins:
    this->CS_pin = PTD_SPI1_CS_NRF24;
    PORTD->PCR[this->CS_pin] |= PORT_PCR_MUX(1); // chip select
    frdm_kl25z_initialize_port_d_output_pin(this->CS_pin);
    // set inactive high.
    GPIOD->PSOR |= (1 << this->CS_pin);

    // set the port d pins to alternate function 2 for peripheral controlled pins
    PORTD->PCR[PTD_SPI1_CLK] |= PORT_PCR_MUX(2);
    PORTD->PCR[PTD_SPI1_MOSI] |= PORT_PCR_MUX(2);
    PORTD->PCR[PTD_SPI1_MISO] |= PORT_PCR_MUX(2);

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


    // enable interrupt for the receive buffer full
    SPI1->C1 |= SPI_C1_SPIE(1);
    // enable the transmitter and receiver

    return status;
}

SPIStatus frdm_kl25z_spi_shutdown(spi_peripheral_t volatile *this)
{
    return SPI_Status_Success;
}

SPIStatus frdm_kl25z_spi_transmit_byte(spi_peripheral_t *this,
                                       const uint8_t byte)
{
    SPIStatus status = SPI_Status_Success;
    frdm_kl25z_spi_state_t state;
    // preserve the current state so we can restore it at the end.
    frdm_kl25z_get_spi_state(&state, this->CS_pin);

    // set chip select active so we can write, inactive high, active low
    GPIOD->PCOR |= (1 << this->CS_pin);
    // poll the status register until empty. SPTEF == 1 --> empty
    bool transmit_buffer_empty = false;
    while (!transmit_buffer_empty) {
        transmit_buffer_empty = SPI1->S & SPI_S_SPTEF(1);
    }

    // send a character
    SPI1->D = byte;
    transmit_buffer_empty = false;
    // white until transmit buffer is empty
    while (!transmit_buffer_empty) {
        transmit_buffer_empty = SPI1->S & SPI_S_SPTEF(1);
    }
    // restore original state
    frdm_kl25z_set_spi_state(&state, this->CS_pin);

    // enable interrupt for the transmit buffer empty
    SPI1->C1 |= SPI_C1_SPTIE(1);

    return status;
}

SPIStatus frdm_kl25z_spi_transmit_n_bytes(spi_peripheral_t *this,
        uint8_t const *const byte, const size_t num_bytes)
{
    SPIStatus status = SPI_Status_Success;
    // set CS active low.
    GPIOD->PCOR |= (1 << this->CS_pin);

    for (size_t i = 0; i < num_bytes; i++) {
        frdm_kl25z_spi_transmit_byte(this, *(byte + i));
    }

    // set CS inactive high.
    GPIOD->PSOR |= (1 << this->CS_pin);
    return status;
}

SPIStatus frdm_kl25z_spi_polling_transmit_receive_byte(
    spi_peripheral_t volatile *this, uint8_t *byte)
{
    SPIStatus status = SPI_Status_Success;
    // preserve the current state so we can restore it at the end.
    frdm_kl25z_spi_state_t state;
    frdm_kl25z_get_spi_state(&state, this->CS_pin);

    // set chip select active so we can write, inactive high, active low
    GPIOD->PCOR |= (1 << this->CS_pin);
    // poll the status register until empty. SPTEF == 1 --> empty

    bool transmit_buffer_empty = false;
    while (!transmit_buffer_empty) {
        transmit_buffer_empty = SPI1->S & SPI_S_SPTEF(1);
    }

    // send a character
    SPI1->D = *byte;

    bool receive_buffer_full = false;
    while (!receive_buffer_full) {
        receive_buffer_full = SPI1->S & SPI_S_SPRF(1);
    }
    *byte = SPI1->D;

    // since this is synchronous transmission, if we receive a byte, the
    // transmit buffer should be empty, but check to be safe
    transmit_buffer_empty = false;
    // white until transmit buffer is empty
    while (!transmit_buffer_empty) {
        transmit_buffer_empty = SPI1->S & SPI_S_SPTEF(1);
    }

    // restore original state of the chip select pin
    frdm_kl25z_set_spi_state(&state, this->CS_pin);

    return status;
}

SPIStatus frdm_kl25z_spi_polling_transmit_receive_n_bytes(
    spi_peripheral_t volatile *this, const size_t num_bytes)
{
    SPIStatus status = SPI_Status_Success;
    // preserve the current state so we can restore it at the end.
    frdm_kl25z_spi_state_t state;
    frdm_kl25z_get_spi_state(&state, this->CS_pin);

    // set CS active low.
    GPIOD->PCOR |= (1 << this->CS_pin);

    uint8_t byte;
    for (size_t i = 0; i < num_bytes; i++) {
        CircularBufferRemoveItem(this->transmit_buffer, &byte);
        this->polling_transmit_receive_byte(this, &byte);
        CircularBufferAddItem(this->receive_buffer, &byte);
    }

    // restore original state of the chip select pin
    frdm_kl25z_set_spi_state(&state, this->CS_pin);

    return status;
}

SPIStatus frdm_kl25z_spi_receive_byte(spi_peripheral_t *this, uint8_t *byte)
{
    SPIStatus status = SPI_Status_Success;
    // poll the status register until full. SPTEF == 1 --> empty
    bool receive_buffer_full = false;
    while (!receive_buffer_full) {
        receive_buffer_full = SPI1->S & SPI_S_SPRF(1);
    }
    *byte = SPI1->D;
    return status;
}


SPIStatus frdm_kl25z_spi_receive_n_bytes(spi_peripheral_t *this, uint8_t *byte,
        const size_t num_bytes)
{
    SPIStatus status = SPI_Status_Success;
    return status;
}

SPIStatus frdm_kl25z_spi_flush_transmit_buffer(spi_peripheral_t *this)
{
    SPIStatus status = SPI_Status_Success;
    bool transmit_buffer_empty = false;
    while (!transmit_buffer_empty) {
        transmit_buffer_empty = SPI1->S & SPI_S_SPTEF(1);
    }
    return status;
}

void frdm_kl25z_spi_begin_async_transmit(void)
{
}

void frdm_kl25z_get_spi_state(frdm_kl25z_spi_state_t *state, const GPIO_PINS pin)
{
    // set chip select, inactive high, active low
    state->chip_select = GPIOD->PDOR & (1 << pin);
}

void frdm_kl25z_set_spi_state(frdm_kl25z_spi_state_t *state, const GPIO_PINS pin)
{
    if (state->chip_select == 0) {
        // active low
        GPIOD->PCOR |= (1 << pin);
    } else {
        // inactive high
        GPIOD->PSOR |= (1 << pin);
    }
}

extern void SPI1_IRQHandler(void)
{
    if (SPI1->S | SPI_S_SPRF_MASK) {
        // read buffer full
        uint8_t byte = SPI1->D;
        // TODO: store by in receive_buffer
        (void)byte;
    } else if (SPI1->S | SPI_S_SPTEF_MASK) {
        // transmit buffer empty
        // TODO: read byte from transmit_buffer;
        uint8_t byte;
        SPI1->D = byte;
    }
}
