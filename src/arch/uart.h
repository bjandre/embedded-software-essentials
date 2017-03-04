#ifndef ESE_ARCH_UART_H_
#define ESE_ARCH_UART_H_

#include <stddef.h>
#include <stdint.h>

#include "uart-common.h"

typedef struct {
    UartStatus (*initialize)(const uint32_t baud);
    UartStatus (*transmit_byte)(const uint8_t byte);
    UartStatus (*transmit_n_bytes)(const size_t num_bytes, uint8_t *bytes);
    UartStatus (*receive_byte)(uint8_t *byte);
    UartStatus (*receive_n_bytes)(const size_t num_bytes, uint8_t *bytes);
} uart_t;

extern uart_t uart;

UartStatus CreateUART();



#endif // ESE_ARCH_UART_H_
