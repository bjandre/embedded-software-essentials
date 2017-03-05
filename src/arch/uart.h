#ifndef ESE_ARCH_UART_H_
#define ESE_ARCH_UART_H_

#include <stddef.h>
#include <stdint.h>

#include "uart-common.h"

typedef struct uart_t uart_t;

struct uart_t {
    UartStatus (*initialize)(const uint32_t baud);
    UartStatus (*transmit_byte)(const uint8_t byte);
    UartStatus (*transmit_n_bytes)(uart_t *uart, const size_t num_bytes,
                                   uint8_t *bytes);
    UartStatus (*receive_byte)(uint8_t *byte);
    UartStatus (*receive_n_bytes)(uart_t *uart, const size_t num_bytes,
                                  uint8_t *bytes);
};

UartStatus CreateUART(uart_t *uart, UartFunction uart_func);



#endif // ESE_ARCH_UART_H_
