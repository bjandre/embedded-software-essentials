/*
 * frdm-kl25z-uart.h
 *
 *  Created on: Mar 3, 2017
 *      Author: andreb
 */

#ifndef ESE_ARCH_UART_FRDM_KL25Z_H_
#define ESE_ARCH_UART_FRDM_KL25Z_H_

#include <stdint.h>

#include "uart-common.h"

/**
   frdm_kl25z_uart_initialize()

   frdm-kl25z specific initialization for the uart. Conforms to the call
   signature of the initialization routine in the uart_t defined in uart.h

   Params: baud - required baud

   Returns: status of the operation
 */
UartStatus frdm_kl25z_uart_initialize(const uint32_t baud);

/**
   frdm_kl25z_uart_transmit_byte()

   frdm-kl25z specific initialization for the uart_transmit_byte. Conforms to
   the call signature of the transmit_byte routine in the uart_t defined in
   uart.h

   Params: byte - the byte to be sent

   Returns: status of the operation
 */
UartStatus frdm_kl25z_uart_transmit_byte(const uint8_t byte);

/**
   frdm_kl25z_uart_receive_byte()

   frdm-kl25z specific initialization for the uart_receive_byte. Conforms to the
   call signature of the receive_byte routine in the uart_t defined in uart.h

   Params: byte - location to store the received byte.

   Returns: status of the operation
 */
UartStatus frdm_kl25z_uart_receive_byte(uint8_t *byte);

#endif /* ESE_ARCH_UART_FRDM_KL25Z_H_ */
