#ifdef BARE_METAL
#define PRINTF(X)
#else
#define PRINTF(ARGS) printf(ARGS)
#include <stdio.h>
#endif

#include <stdint.h>
#include <stdlib.h>

#include "logger.h"

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

    BinaryLoggerStatus logger_status = BinaryLogger_OK;
 logger_status = BinaryLoggerInitialize(32);

    uint8_t *buffer = malloc(sizeof(uint8_t) * 32);
    uint8_t byte;
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
            byte = 0x55u;
            log_data(sizeof(uint8_t), &byte);
            log_data(sizeof(data1), (uint8_t *)(&data1));
            byte = 0xAAu;
            log_data(sizeof(uint8_t), &byte);
            log_data(sizeof(data2), (uint8_t *)(&data2));
        } else {
            log_receive_data(sizeof(byte), &byte);
            switch (byte) {
            case '1':
                log_data(sizeof(data1), (uint8_t *)(&data1));
                break;
            case '2':
                log_data(sizeof(data2), (uint8_t *)(&data2));
                break;
            case '3':
                log_data(sizeof(data3), (uint8_t *)(&data3));
                break;
            case 'a':
                log_receive_data(sizeof(byte), &byte);
                uint8_t num_bytes = byte - '0';
                log_receive_data((size_t)num_bytes, buffer);
                log_data(sizeof(num_bytes), (uint8_t *)(&num_bytes));
                log_data(num_bytes, buffer);
                break;
            default:
                log_data(sizeof(byte), &byte);
            }
        }
    }
    free(buffer);
    return 0;
}
