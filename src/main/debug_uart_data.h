
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef ESE_MAIN_DEBUG_UART_DATA_H_
#define ESE_MAIN_DEBUG_UART_DATA_H_

#include <stddef.h>
#include <stdint.h>

/* some interesting byte patterns to send over uart. */
static const uint32_t data_dead_beef = 0xEFBEADDEu;
static const uint32_t data_dead_code = 0xDEC0ADDEu;
static const uint32_t data_aaaa_5555 = 0x5555AAAAu;
static const uint32_t data_negative_int = -40577;
static const char ese_hello[] =
    "Hello from Emebbed Software Essentials Project!\n";


/**
   debug_uart()

   Routine to send and receive some data for debugging the uart logger.

   \param[in] data_switch - flag to control which uart data test is run.
   \param[in] *buffer - pointer to data buffer
   \param[in] buffer_size

 */
void debug_uart(uint8_t data_switch, uint8_t *buffer, size_t buffer_size);

/**
   debug_uart_transmit()

   Routine to send some fixed byte patterns to the logger

 */
void debug_uart_transmit(void);

/**
   debug_uart()

   Routine to receive a byte and send back different data based on the received
   data. Note: this only works if the logging algorithm is set to polling.

   \param[in] *buffer - pointer to buffer to received and transmit
   \param[in] buffer_size size of the buffer

 */
void debug_uart_receive_transmit(uint8_t *buffer, size_t buffer_size);

#endif/* ESE_MAIN_DEBUG_UART_DATA_H_ */
