#include <stdint.h>
#include <stdio.h>

#include "uart-common.h"
#include "uart-host.h"


UartStatus host_uart_initialize(const uint32_t baud)
{
    // nothing to do
    UartStatus status = UART_Status_OK;
    return status;
}

UartStatus host_uart_transmit_byte(const uint8_t byte)
{
    UartStatus status = UART_Status_OK;
    fprintf(stdout, "%0.2x", byte);
    //putc(byte, stdout);
    return status;
}

UartStatus host_uart_receive_byte(uint8_t *byte)
{
    UartStatus status = UART_Status_OK;
    *byte = fgetc(stdin);
    // fgetc reads a single character. Terminal input will contain an extra
    // character for the newline. Need to remove it with another fgetc.
    if ('\n' == *byte) {
        // user input just a newline, do nothing
    } else {
        fgetc(stdin);
    }
    return status;
}
