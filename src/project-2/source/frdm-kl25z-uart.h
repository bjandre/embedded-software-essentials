/*
 * frdm-kl25z-uart.h
 *
 *  Created on: Mar 3, 2017
 *      Author: andreb
 */

#ifndef SOURCE_FRDM_KL25Z_UART_H_
#define SOURCE_FRDM_KL25Z_UART_H_

#include <stddef.h>
#include <stdint.h>


// this should be moved into a generic uart header so status is the same
// regardless of hardware platform specifics.
typedef enum UartStatus_t {
	UART_Status_OK,
	UART_Status_Error,
} UartStatus;

UartStatus frdm_kl25z_uart_initialize(const uint32_t baud_rate);
UartStatus frdm_kl25z_uart_transmit_byte(const uint8_t byte);
UartStatus frdm_kl25z_uart_transmit_n_bytes(const size_t num_bytes, uint8_t *bytes);

#endif /* SOURCE_FRDM_KL25Z_UART_H_ */
