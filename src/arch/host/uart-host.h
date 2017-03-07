#ifndef ESE_ARCH_HOST_UART_H_
#define ESE_ARCH_HOST_UART_H_

#include <stdint.h>

#include "uart-common.h"

UartStatus host_uart_initialize(const uint32_t baud);
UartStatus host_uart_transmit_byte(const uint8_t byte);
UartStatus host_uart_receive_byte(uint8_t *byte);



#endif // ESE_ARCH_HOST_UART_H_
