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

UartStatus frdm_kl25z_uart_initialize(const uint32_t baud);
UartStatus frdm_kl25z_uart_transmit_byte(const uint8_t byte);
UartStatus frdm_kl25z_uart_receive_byte(uint8_t *byte);

#endif /* ESE_ARCH_UART_FRDM_KL25Z_H_ */
