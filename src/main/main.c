#ifdef BARE_METAL
#define PRINTF(X)
#else
#define PRINTF(ARGS) printf(ARGS)
#include <stdio.h>
#endif

#include <stdint.h>
#include <stdlib.h>

#include "platform-defs.h"
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

#if (PLATFORM == PLATFORM_FRDM)
#include "MKL25Z4.h"
void initialize_led_pin(uint8_t led_pin);
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
    if (BinaryLogger_OK != logger_status) {
        abort();
    }

#if (PLATFORM == PLATFORM_FRDM)
    // enable clock for gpio led pins.
    SIM->SCGC5 |= SIM_SCGC5_PORTB(1);

    const uint32_t led_red_pin = 18;
    const uint32_t led_green_pin = 19;
    initialize_led_pin(led_red_pin);
    initialize_led_pin(led_green_pin);
    // toggle led pins
    GPIOB->PTOR |= (1 << led_green_pin);
#endif

    uint8_t *buffer = malloc(sizeof(uint8_t) * 32);
    uint8_t byte;
    uint32_t data1 = 0xEFBEADDEu;
    uint32_t data2 = 0xDEC0ADDEu;
    uint32_t data3 = 0x5555AAAAu;
    /* Add your code here */
    uint8_t tx_or_rx = 0;

    while (1) { /* Infinite loop to avoid leaving the main function */
        __asm("NOP"); /* something to use as a breakpoint stop while looping */
#if (PLATFORM == PLATFORM_FRDM)
        for (uint32_t i = 0; i < 200000; i++) {
            // do nothing for a while.
        }
        // toggle led pins
        GPIOB->PTOR |= (1 << led_red_pin);
        GPIOB->PTOR |= (1 << led_green_pin);
#endif
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

#if (PLATFORM == PLATFORM_FRDM)
void initialize_led_pin(uint8_t led_pin)
{
    // Initialize the gpio pin for the led
    PORTB->PCR[led_pin] = PORT_PCR_MUX(1);
    // set pin to output
    GPIOB->PDDR |= (1 << led_pin);
    // initial state of pin is on
    GPIOB->PDOR |= (1 << led_pin);
}
#endif
