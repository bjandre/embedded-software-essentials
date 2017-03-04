#include <stddef.h>
#include <stdint.h>

#include "uart.h"

#if PLATFORM == host
#include "uart-host.h"
#elif PLATFORM == frdm
#include "uart-frdm-kl25z.h"
#endif

UartStatus uart_transmit_n_bytes(const size_t num_bytes, uint8_t *bytes);
UartStatus uart_receive_n_bytes(const size_t num_bytes, uint8_t *bytes);

uart_t uart;

UartStatus uart_transmit_n_bytes(const size_t num_bytes, uint8_t *bytes)
{
    UartStatus status = UART_Status_OK;
    for (size_t n = 0; n < num_bytes; n++) {
        uart.transmit_byte(*(bytes + n));
    }
    return status;
}

UartStatus uart_receive_n_bytes(const size_t num_bytes, uint8_t *bytes)
{
    UartStatus status = UART_Status_OK;
    for (size_t n = 0; n < num_bytes; n++) {
        uart.receive_byte(bytes + n);
    }
    return status;
}


UartStatus CreateUART(void)
{
    UartStatus status = UART_Status_OK;

#if PLATFORM == host
    uart.initialize = &host_uart_initialize;
    uart.transmit_byte = &host_uart_transmit_byte;
    uart.transmit_n_bytes = &uart_transmit_n_bytes;
    uart.receive_byte = &host_uart_receive_byte;
    uart.receive_n_bytes = &uart_receive_n_bytes;
#elif PLATFORM == frdm
    uart.initialize = &frdm_kl25z_uart_initialize;
    uart.transmit_byte = &frdm_kl25z_uart_transmit_byte;
    uart.transmit_n_bytes = &uart_transmit_n_bytes;
    uart.receive_byte = &frdm_kl25z_uart_receive_byte;
    uart.recieve_n_bytes = &uart_receive_n_bytes;
#elif PLATFORM == bbb
#error "ERROR: UART not implemented for PLATFORM == bbb"
#endif

    return status;
}

