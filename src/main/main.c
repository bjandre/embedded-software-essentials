#ifdef BARE_METAL
#define PRINTF(X)
#else
#define PRINTF(ARGS) printf(ARGS)
#include <stdio.h>
#endif

#include <stdint.h>
#include <stdlib.h>

#include "circular_buffer.h"
#include "uart.h"

#if PROJECT == 0
#    // NOTE(bja, 2017-02): Building main with no project code.
#elif PROJECT == 1
#    include "project_1.h"
#elif PROJECT == 2
#
#else
#    error "Unsupported project number in PROJECT macro. Valid values: 0, 1"
#endif

int main(int argc, char **argv)
{

    PRINTF("Hello, from Emebbed Software Essentials Project!\n");

#if PROJECT == 1
    project_1_report();
#elif PROJECT == 2
#
#endif

    size_t bytes_per_item = 1;
    size_t num_items = 32;

    CircularBuffer_t *cb_1byte = NULL;
    CircularBufferStatus status = CircularBufferNew(&cb_1byte, num_items,
                                  bytes_per_item);
    if (CB_No_Error == status) {
        // FIXME(bja, 2017-02) convert to logging infrastructure.
        PRINTF("Circular buffer initialized!\n");
    } else {
        PRINTF("Error initializing circular buffer!\n");
    }

    UartStatus uart_status = CreateUART();
    if (UART_Status_Error == uart_status) {
        PRINTF("Error creating UART!\n");
    }

    uint32_t baud = 115200u;
    uart_status = uart.initialize(baud);
    if (UART_Status_Error == uart_status) {
        PRINTF("Error initializing UART!\n");
    } else {
        PRINTF("UART initialized!\n");
    }

    uint8_t *buffer = malloc(sizeof(uint8_t) * 32);
    uint32_t data1 = 0xEFBEADDEu;
    uint32_t data2 = 0xDEC0ADDEu;
    uint32_t data3 = 0x5555AAAAu;
    /* Add your code here */
    uint8_t tx_or_rx = 0;

    while (1) { /* Infinite loop to avoid leaving the main function */
        __asm("NOP"); /* something to use as a breakpoint stop while looping */
        for (uint32_t i = 0; i < 200000; i++) {
            // do nothing for a while.
        }

        if (tx_or_rx) {
            uart.transmit_byte((uint8_t)0x55);
            uart.transmit_n_bytes(sizeof(uint32_t), (uint8_t *)(&data1));
            uart.transmit_byte((uint8_t)0xAA);
            uart.transmit_n_bytes(sizeof(uint32_t), (uint8_t *)(&data2));
        } else {
            uint8_t byte;
            uart.receive_byte(&byte);
            switch (byte) {
            case '1':
                uart.transmit_n_bytes(sizeof(uint32_t), (uint8_t *)(&data1));
                break;
            case '2':
                uart.transmit_n_bytes(sizeof(uint32_t), (uint8_t *)(&data2));
                break;
            case '3':
                uart.transmit_n_bytes(sizeof(uint32_t), (uint8_t *)(&data3));
                break;
            case 'a':
                uart.receive_byte(&byte);
                size_t num_bytes = byte - '0';
                uart.receive_n_bytes(num_bytes, buffer);
                uart.transmit_byte(num_bytes);
                uart.transmit_n_bytes(num_bytes, buffer);
                break;
            default:
                uart.transmit_byte(byte);
            }
        }
    }
    free(buffer);
    return 0;
}
