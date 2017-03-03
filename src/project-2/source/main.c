/*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * This is template for main module created by New Kinetis SDK 2.x Project Wizard. Enjoy!
 **/

#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"

#include "MKL25Z4.h"
#include "system_MKL25Z4.h"

/*!
 * @brief Application entry point.
 */
int main(void) {
  /* Init board hardware. */
  BOARD_InitPins();
  BOARD_BootClockRUN();
  BOARD_InitDebugConsole();

  /* Add your code here */

  // enable clock for gpio led pin.
  SIM->SCGC5 |= SIM_SCGC5_PORTB(1);

  const uint32_t led_pin = 19;
  // Initialize the gpio pin for the led
  PORTB->PCR[led_pin] = PORT_PCR_MUX(1);
  // set pin to output
  GPIOB->PDDR |= (1 << led_pin);
  // initial state of pin is on
  GPIOB->PDOR |= (1 << led_pin);


  // UART0
  //   PTA1 - Port A, Pin 1, Alternate function 2, UART0_RX
  //   PTA2 - Port A, Pin 2, Alternate function 2, UART0_TX

  // enable the clock for porta pins
  SIM->SCGC5 |= SIM_SCGC5_PORTA(1);
  SIM->SCGC4 |= SIM_SCGC4_UART0(1);  // enable uart0

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

  // set baud rate
  uint32_t baud_rate = 19200U;
  //uint32_t over_sampling_rate = (uint32_t)(UART0->C4 & UART0_C4_OSR_MASK);
  uint32_t over_sampling_rate = 3;
  uint32_t br_clock = SystemCoreClock;
  // baud rate is 13 bit field in registers
  uint16_t baud_rate_reg = br_clock / ((over_sampling_rate + 1) * baud_rate);
  // shift off the lowest byte and mask off the 5 bits
  uint8_t high_mask = ((baud_rate_reg >> 8) & UART0_BDH_SBR_MASK);
  UART0->BDH |= high_mask; // set ones
  UART0->BDH &= ~high_mask; // set zeros
  // just mask off the lowest byte. since we want all eight bits, just assign it all.
  uint8_t low_mask = (baud_rate_reg & UART0_BDL_SBR_MASK);
  UART0->BDL = low_mask;
  UART0->C4 |= UART0_C4_OSR(over_sampling_rate);
  UART0->C4 &= ~UART0_C4_OSR(over_sampling_rate);

  // set 1 stop bit
  UART0->BDH &= ~UART0_BDH_SBNS(0);

  // set number of data bits
  UART0->C1 &= ~UART0_C1_M(0); // 8 bit

  // set parity
  UART0->C1 &= ~UART0_C1_PE(0); // no parity
  //UART0->C1 = 0x00u;
  UART0->C3 = 0x00u;
  UART0->S2 = 0x00u;
  //UART0->C5 |= UART0_C5_BOTHEDGE(1);

  // enable the transmitter and receiver
  UART0->C2 |= UART0_C2_TE(1);
  UART0->C2 |= UART0_C2_RE(1);

  BOARD_InitDebugConsole();

  /* Add your code here */

  for (;;) { /* Infinite loop to avoid leaving the main function */
      __asm("NOP"); /* something to use as a breakpoint stop while looping */
      for (uint32_t i = 0; i < 200000; i++) {
          // do nothing for a while.
      }
      GPIOB->PTOR |= (1 << led_pin); // toggle led pin
      while ((UART0->S1 & UART0_S1_TDRE_MASK) == 0);
      UART0->D = 0x55; // send a character
  }
}
