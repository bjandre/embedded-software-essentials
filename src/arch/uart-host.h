#ifndef ESE_ARCH_HOST_UART_H_
#define ESE_ARCH_HOST_UART_H_

#include <stdint.h>

#include <stddef.h>
#include <stdint.h>


UartStatus host_uart_initialize(const uint32_t baud);
UartStatus host_uart_transmit_byte(const uint8_t byte);
UartStatus host_uart_transmit_n_bytes(const size_t num_bytes, uint8_t *bytes);
UartStatus host_uart_receive_byte(uint8_t *byte);
UartStatus host_uart_receive_n_bytes(const size_t num_bytes, uint8_t *bytes);



#endif // ESE_ARCH_HOST_UART_H_
