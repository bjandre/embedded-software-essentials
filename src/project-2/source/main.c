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
#include <assert.h>
#include <stdlib.h>

#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"

#include "frdm-kl25z-uart.h"

#include "MKL25Z4.h"
#include "system_MKL25Z4.h"

void initialize_led_pin(uint8_t led_pin);

/*!
 * @brief Application entry point.
 */
int main(void) {
  /* Init board hardware. */
  BOARD_InitPins();
  BOARD_BootClockRUN();
  BOARD_InitDebugConsole();

  /* Add your code here */

  // enable clock for gpio led pins.
  SIM->SCGC5 |= SIM_SCGC5_PORTB(1);

  const uint32_t led_red_pin = 18;
  const uint32_t led_green_pin = 19;
  initialize_led_pin(led_red_pin);
  initialize_led_pin(led_green_pin);
  // toggle led pins
  GPIOB->PTOR |= (1 << led_green_pin);
  // setup the uart debugging interface
  uint32_t baud = 115200u;
  UartStatus status = frdm_kl25z_uart_initialize(baud);
  if (status != UART_Status_OK) {
      // assert and die? infinite loop?
  }

  BOARD_InitDebugConsole();
  uint8_t *buffer = malloc(sizeof(uint8_t) * 32);
  uint32_t data1 = 0xEFBEADDEu;
  uint32_t data2 = 0xDEC0ADDEu;
  uint32_t data3 = 0x5555AAAAu;
  /* Add your code here */
  uint8_t tx_or_rx = 0;
  for (;;) { /* Infinite loop to avoid leaving the main function */
      __asm("NOP"); /* something to use as a breakpoint stop while looping */
      for (uint32_t i = 0; i < 200000; i++) {
          // do nothing for a while.
      }
      // toggle led pins
      GPIOB->PTOR |= (1 << led_red_pin);
      GPIOB->PTOR |= (1 << led_green_pin);
      if (tx_or_rx) {
          frdm_kl25z_uart_transmit_byte((uint8_t)0x55);
          frdm_kl25z_uart_transmit_n_bytes(sizeof(uint32_t), (uint8_t*)(&data1));
          frdm_kl25z_uart_transmit_byte((uint8_t)0xAA);
          frdm_kl25z_uart_transmit_n_bytes(sizeof(uint32_t), (uint8_t*)(&data2));
      } else {
          uint8_t byte;
          frdm_kl25z_uart_receive_byte(&byte);
          switch (byte) {
          case '1':
              frdm_kl25z_uart_transmit_n_bytes(sizeof(uint32_t), (uint8_t*)(&data1));
              break;
          case '2':
              frdm_kl25z_uart_transmit_n_bytes(sizeof(uint32_t), (uint8_t*)(&data2));
              break;
          case '3':
              frdm_kl25z_uart_transmit_n_bytes(sizeof(uint32_t), (uint8_t*)(&data3));
              break;
          case 'a':
              frdm_kl25z_uart_receive_byte(&byte);
              size_t num_bytes = byte - '0';
              frdm_kl25z_uart_receive_n_bytes(num_bytes, buffer);
              frdm_kl25z_uart_transmit_byte(num_bytes);
              frdm_kl25z_uart_transmit_n_bytes(num_bytes, buffer);
              break;
          default:
              frdm_kl25z_uart_transmit_byte(byte);
          }
      }
      assert(1);
  }
  free(buffer);
}

void initialize_led_pin(uint8_t led_pin)
{
    // Initialize the gpio pin for the led
    PORTB->PCR[led_pin] = PORT_PCR_MUX(1);
    // set pin to output
    GPIOB->PDDR |= (1 << led_pin);
    // initial state of pin is on
    GPIOB->PDOR |= (1 << led_pin);
}

