
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

/*
 * uart.c
 *
 *  Created on: Mar 3, 2017
 *      Author: andreb
 */

#include <stddef.h>
#include <stdint.h>

#include "MKL25Z4.h"
#include "system_MKL25Z4.h"

#include "uart-frdm-kl25z.h"

#include "circular_buffer.h"

UartStatus frdm_kl25z_uart_initialize(const uint32_t baud)
{
    // UART0
    //   PTA1 - Port A, Pin 1, Alternate function 2, UART0_RX
    //   PTA2 - Port A, Pin 2, Alternate function 2, UART0_TX

    // enable the clock for porta pins
    UartStatus status = UART_Status_OK;
    SIM->SCGC5 |= SIM_SCGC5_PORTA(1);
    SIM->SCGC4 |= SIM_SCGC4_UART0(1);  // enable uart0

    SIM->SOPT2 &= ~SIM_SOPT2_PLLFLLSEL(1);
    SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1); // use the MGC FLL or PLL/2

    // set the rx tx source to be the pins on port 1
    PORTA->PCR[1] |= PORT_PCR_MUX(2);
    PORTA->PCR[2] |= PORT_PCR_MUX(2);
    SIM->SOPT5 &= ~SIM_SOPT5_UART0TXSRC(0);
    SIM->SOPT5 &= ~SIM_SOPT5_UART0RXSRC(0);

    // make sure the transmitter and receiver are disabled so we can change the config.
    UART0->C2 |= UART0_C2_TE(1);
    UART0->C2 |= UART0_C2_RE(1);

    // NOTE(bja, 2017-02) uart registers are 8 bit!

    // setup for the specified baud rate
    //uint32_t over_sampling_rate = (uint32_t)(UART0->C4 & UART0_C4_OSR_MASK);
    uint32_t over_sampling_rate = 15;
    uint32_t baud_clock = SystemCoreClock;
    // baud rate is 13 bit field in registers
    uint16_t baud_reg = baud_clock / ((over_sampling_rate + 1) * baud);
    // shift off the lowest byte and mask off the 5 bits
    uint8_t high_mask = ((baud_reg >> 8) & UART0_BDH_SBR_MASK);
    UART0->BDH |= high_mask; // set ones
    UART0->BDH &= ~high_mask; // set zeros
    // just mask off the lowest byte. since we want all eight bits, just assign it all.
    uint8_t low_mask = (baud_reg & UART0_BDL_SBR_MASK);
    UART0->BDL = low_mask;
    UART0->C4 |= UART0_C4_OSR(over_sampling_rate);
    UART0->C4 &= ~UART0_C4_OSR(over_sampling_rate);

    // set 1 stop bit
    UART0->BDH &= ~UART0_BDH_SBNS(0);

    // set number of data bits
    UART0->C1 &= ~UART0_C1_M(0); // 8 bit

    // set parity
    UART0->C1 &= ~UART0_C1_PE(0); // no parity

    UART0->C3 = 0x00u;
    UART0->S2 = 0x00u;

    UART0->C5 |= UART0_C5_BOTHEDGE(1);

    // enable interrupts for the transmitter and receiver
    UART0->C2 |= UART0_C2_RIE(1);
    //UART0->C2 |= UART0_C2_TIE(1);

    // enable the transmitter and receiver
    UART0->C2 |= UART0_C2_TE(1);
    UART0->C2 |= UART0_C2_RE(1);

    return status;
}

UartStatus frdm_kl25z_uart_transmit_byte(const uint8_t byte)
{
    const uint8_t TX_BUFFER_FULL = 0;
    UartStatus status = UART_Status_OK;
    // FIXME(bja, 2017-02) is uart0->d shared between transmit and receive?
    UART0->D = byte; // send a character
    while ((UART0->S1 & UART0_S1_TDRE_MASK) ==
            TX_BUFFER_FULL); // ensure the transmit data register is empty
    return status;
}

UartStatus frdm_kl25z_uart_transmit_n_bytes(const size_t num_bytes,
        uint8_t *bytes)
{
    UartStatus status = UART_Status_OK;
    for (size_t n = 0; n < num_bytes; n++) {
        frdm_kl25z_uart_transmit_byte(*(bytes + n));
    }
    return status;
}

UartStatus frdm_kl25z_uart_receive_byte(uint8_t *byte)
{
    const uint8_t RX_BUFFER_EMPTY = 0;
    //const uint8_t RX_BUFFER_FULL = 1;
    UartStatus status = UART_Status_OK;
    // FIXME(bja, 2017-02) is uart0->d shared between transmit and receive?
    while ((UART0->S1 & UART0_S1_RDRF_MASK) ==
            RX_BUFFER_EMPTY); // ensure the receive data register is empty
    *byte = UART0->D; // receive a character
    //while ((UART0->S1 & UART0_S1_RDRF_MASK) == RX_BUFFER_FULL); // ensure the receive data register is empty
    return status;
}

UartStatus frdm_kl25z_uart_receive_n_bytes(const size_t num_bytes,
        uint8_t *bytes)
{
    UartStatus status = UART_Status_OK;
    for (size_t n = 0; n < num_bytes; n++) {
        frdm_kl25z_uart_receive_byte(bytes + n);
    }
    return status;
}
