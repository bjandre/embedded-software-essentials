#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "uart.h"
#include "platform-defs.h"

#if (PLATFORM == PLATFORM_HOST) || (PLATFORM == PLATFORM_BBB)
#include "uart-host.h"
#elif (PLATFORM == PLATFORM_FRDM)
#include "uart-frdm-kl25z.h"
#endif

/**
   uart_transmit_n_bytes()

   Platform independent routine to loop over n bytes and transmit them.

   Params: uart - pointer to the uart data structure with platform specific
                  function pointers.
           num_bytes - the number of bytes to transmit
           bytes - pointer to the buffer containing the data

   Returns: status of the operation
 */
UartStatus uart_transmit_n_bytes(uart_t *uart,
                                 const size_t num_bytes, uint8_t *bytes);


/**
   uart_receive_n_bytes()

   Platform independent routine to stort n bytes from the uart receiver

   Params: uart - pointer to the uart data structure with platform specific
                  function pointers.
           num_bytes - the number of bytes to receive
           bytes - pointer to the buffer where the data should be stored

   Returns: status of the operation
 */
UartStatus uart_receive_n_bytes(uart_t *uart,
                                const size_t num_bytes, uint8_t *bytes);


/**
   uartSetupDebugger()

   Setup the platform specific UART for the debugging logger.

   Params: uart - pointer to the uart data structure with platform specific
                  function pointers.

   Returns: status of the operation
 */
UartStatus uartSetupDebugger(uart_t volatile *uart);



UartStatus uart_transmit_n_bytes(uart_t *uart,
                                 const size_t num_bytes, uint8_t *bytes)
{
    UartStatus status = UART_Status_OK;
    for (size_t n = 0; n < num_bytes; n++) {
        uart->transmit_byte(*(bytes + n));
    }
    return status;
}

UartStatus uart_receive_n_bytes(uart_t *uart, const size_t num_bytes,
                                uint8_t *bytes)
{
    UartStatus status = UART_Status_OK;
    for (size_t n = 0; n < num_bytes; n++) {
        uart->receive_byte(bytes + n);
    }
    return status;
}


UartStatus CreateUART(uart_t volatile *uart, UartFunction uart_function)
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

UartStatus uartSetupDebugger(uart_t volatile *uart)
{
    UartStatus status = UART_Status_OK;
#if (PLATFORM == PLATFORM_HOST) || (PLATFORM == PLATFORM_BBB)
    uart->initialize = &host_uart_initialize;
    uart->transmit_byte = &host_uart_transmit_byte;
    uart->receive_byte = &host_uart_receive_byte;
#elif PLATFORM == PLATFORM_FRDM
    uart->initialize = &frdm_kl25z_uart_initialize;
    uart->transmit_byte = &frdm_kl25z_uart_transmit_byte;
    uart->receive_byte = &frdm_kl25z_uart_receive_byte;
#else
#error "ERROR: UART not implemented for unknown PLATFORM " PLATFORM
#endif
    return status;
}
