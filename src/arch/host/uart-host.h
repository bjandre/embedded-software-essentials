#ifndef ESE_ARCH_HOST_UART_H_
#define ESE_ARCH_HOST_UART_H_

#include <stdint.h>

#include "uart-common.h"

/**
   host_uart_initialize()

   Host specific initialization for the uart. Conforms to the call signature of
   the initialization routine in the uart_t defined in uart.h

   Params: baud - required baud

   Returns: status of the operation
 */
UartStatus host_uart_initialize(const uint32_t baud);

/**
   host_uart_transmit_byte()

   Host specific initialization for the uart_transmit_byte. Conforms to the call signature of
   the transmit_byte routine in the uart_t defined in uart.h

   Params: byte - the byte to be sent

   Returns: status of the operation
 */
UartStatus host_uart_transmit_byte(const uint8_t byte);

/**
   host_uart_receive_byte()

   Host specific initialization for the uart_receive_byte. Conforms to the call signature of
   the receive_byte routine in the uart_t defined in uart.h

   Params: byte - location to store the received byte.

   Returns: status of the operation
 */
UartStatus host_uart_receive_byte(uint8_t *byte);



#endif // ESE_ARCH_HOST_UART_H_
