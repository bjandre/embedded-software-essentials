#ifndef ESE_ARCH_UART_COMMON_H_
#define ESE_ARCH_UART_COMMON_H_

#include <stdint.h>

#include "uart-common.h"

typedef enum UartStatus_t {
    UART_Status_OK,
    UART_Status_Error,
} UartStatus;

typedef enum UartFunction_t {
    UartDebugger,
    UartGeneral,
} UartFunction;

#endif // ESE_ARCH_UART_COMMON_H_
