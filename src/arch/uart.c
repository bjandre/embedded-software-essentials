#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "uart.h"

#if PLATFORM == host
#include "uart-host.h"
#elif PLATFORM == frdm
#include "uart-frdm-kl25z.h"
#endif

UartStatus uart_transmit_n_bytes(uart_t *uart,
                                 const size_t num_bytes, uint8_t *bytes);
UartStatus uart_receive_n_bytes(uart_t *uart,
                                const size_t num_bytes, uint8_t *bytes);
UartStatus uartSetupDebugger(uart_t *uart);

UartStatus uart_transmit_n_bytes(uart_t *uart,
                                 const size_t num_bytes, uint8_t *bytes)
{
    UartStatus status = UART_Status_OK;
    for (size_t n = 0; n < num_bytes; n++) {
        uart->transmit_byte(*(bytes + n));
    }
    return status;
}

UartStatus uart_receive_n_bytes(uart_t *uart, const size_t num_bytes, uint8_t *bytes)
{
    UartStatus status = UART_Status_OK;
    for (size_t n = 0; n < num_bytes; n++) {
        uart->receive_byte(bytes + n);
    }
    return status;
}


UartStatus CreateUART(uart_t *uart, UartFunction uart_function)
{
    UartStatus status = UART_Status_OK;

    uart->transmit_n_bytes = &uart_transmit_n_bytes;
    uart->receive_n_bytes = &uart_receive_n_bytes;
    if (UartDebugger == uart_function) {
        uartSetupDebugger(uart);
    } else {
        // FIXME(bja, 2017-03) Additional UART functionality not supported at
        // this time.
        abort();
    }

    return status;
}

UartStatus uartSetupDebugger(uart_t *uart)
{
    UartStatus status = UART_Status_OK;
#if PLATFORM == host || PLATFORM == bbb
    uart->initialize = &host_uart_initialize;
    uart->transmit_byte = &host_uart_transmit_byte;
    uart->receive_byte = &host_uart_receive_byte;
#elif PLATFORM == frdm
    uart->initialize = &frdm_kl25z_uart_initialize;
    uart->transmit_byte = &frdm_kl25z_uart_transmit_byte;
    uart->receive_byte = &frdm_kl25z_uart_receive_byte;
#else
#error "ERROR: UART not implemented for unknown PLATFORM " PLATFORM
#endif
    return status;
}
