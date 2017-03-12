
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifdef BARE_METAL
#define PRINTF(X)
#else
#define MOCK_RECEIVE_DATA_INTERRUPT
#define PRINTF(ARGS) printf(ARGS)
#include <stdio.h>
#endif

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "platform-defs.h"
#include "logger.h"

#include "analyze-data.h"
#include "debug-uart-data.h"

#include "async-global.h"

// Global, asynchronously accessed data instances
volatile BinaryLogger_t logger;
volatile async_data_t global_async_data;

// define the pins for the leds. FIXME(bja, 2017-03) Should be platform
// dependent!
typedef enum GPIO_PINS {
    LED_PIN_RED = 18,
    LED_PIN_GREEN = 19,
} GPIO_PINS;

/*
  initialize_gpio()

  Generic routine to initialize GPIO.

  Wrapper around platform specific code.

  Parameters: none

  Return: none
 */
void initialize_gpio(void);

/*
  initialize_led_pin()

  Generic routine to initialize an led pin.

  Wrapper around platform specific code.

  Parameters: led pin enumeration

  Return: none
 */
void initialize_led_pin(GPIO_PINS led_pin);

/*
  update_leds()

  Generic routine to update led status.

  Wrapper around platform specific code.

  Parameters: none

  Return: none
 */
void update_leds(void);

#if (PLATFORM == PLATFORM_FRDM)
#include "MKL25Z4.h"

/*
  frdm_kl25z_initialize_gpio()

  Platform specific code to initialize GPIO.

  Parameters: none

  Return: none
 */
void frdm_kl25z_initialize_gpio(void);

/*
  frdm_kl25z_initialize_leds()

  Platform specific code to initialize leds.

  Parameters: gpio pin for the led

  Return: none
 */
void frdm_kl25z_initialize_led_pin(GPIO_PINS led_pin);

/*
  frdm_kl25z_update_leds()

  Platform specific code to update led state

  Parameters: none

  Return: none
 */
void frdm_kl25z_update_leds(void);
#endif


int main(int argc, char **argv)
{
    //PRINTF("Hello from Emebbed Software Essentials Project!\n");

    global_async_data.data_available = false;

#if (PLATFORM == PLATFORM_FRDM)
    NVIC_EnableIRQ(UART0_IRQn);
#endif

    BinaryLoggerStatus logger_status = BinaryLogger_OK;
    uint8_t const buffer_size_bytes = 32;
    logger_status = BinaryLoggerInitialize(buffer_size_bytes);
    if (BinaryLogger_OK != logger_status) {
        abort();
    }

    log_item_t *item;
    logger_status = CreateLogItem(&item);
    if (BinaryLogger_OK != logger_status) {
        abort();
    }
    logger_status = UpdateLogItem(item, LOGGER_INITIALIZED, zero_payload_bytes,
                                  null_payload);
    if (BinaryLogger_OK != logger_status) {
        abort();
    }
    log_item(item);

    initialize_gpio();

    logger_status = UpdateLogItem(item, SYSTEM_INITIALIZED, zero_payload_bytes,
                                  null_payload);
    if (BinaryLogger_OK != logger_status) {
        abort();
    }
    log_item(item);

#undef DEBUG_UART
#ifdef DEBUG_UART
    size_t const buffer_size = 32 * sizeof(uint8_t);
    uint8_t *buffer = malloc(buffer_size);
#endif

    uint8_t byte;
    uint8_t const num_required = 16;
    uint8_t num_received = 0;
    data_summary_t data_summary;
    bool data_available;
    clear_data_summary(&data_summary);

    logger_status = UpdateLogItem(item, DATA_ANALYSIS_STARTED, zero_payload_bytes,
                                  null_payload);
    if (BinaryLogger_OK != logger_status) {
        abort();
    }
    log_item(item);

    while (1) { /* main event loop */
        __asm("NOP"); /* breakpoint to stop while looping */
        update_leds();

#ifdef DEBUG_UART
        uint8_t tx_or_rx = 1;
        debug_uart(tx_or_rx, buffer, buffer_size);
#endif

#ifdef MOCK_RECEIVE_DATA_INTERRUPT
        global_async_data.data_available = true;
#endif

        {
            // NOTE(bja, 2017-03) critical region accessing global data.
            data_available = global_async_data.data_available;
            global_async_data.data_available = false;
        }

        if (data_available) {
            log_receive_data(1, &byte);
            logger_status = UpdateLogItem(item, DATA_RECEIVED, 1, &byte);
            if (BinaryLogger_OK != logger_status) {
                abort();
            }
            log_item(item);
            num_received++;
            process_data(&data_summary, byte);
        }

        if (num_received == num_required) {
            log_data_analysis(item, &data_summary);
            logger_status = UpdateLogItem(item, DATA_ANALYSIS_COMPLETED,
                                          zero_payload_bytes, null_payload);
            if (BinaryLogger_OK != logger_status) {
                abort();
            }
            log_item(item);
            clear_data_summary(&data_summary);
            num_received = 0;
#ifdef MOCK_RECEIVE_DATA_INTERRUPT
            abort();
#endif
        }
    }
#ifdef DEBUG_UART
    free(buffer);
#endif
    logger_status = DestroyLogItem(&item);
    return 0;
}

void initialize_gpio(void)
{
#if (PLATFORM == PLATFORM_FRDM)
    frdm_kl25z_initialize_gpio();
#endif
}
void initialize_led_pin(GPIO_PINS led_pin)
{
#if (PLATFORM == PLATFORM_FRDM)
    frdm_kl25z_initialize_led_pin(led_pin);
#else
    (void)led_pin;
#endif
}

void update_leds(void)
{
#if (PLATFORM == PLATFORM_FRDM)
    frdm_kl25z_update_leds();
#endif
}

#if (PLATFORM == PLATFORM_FRDM)
//
// platform specific code for the frdm-kl25z
//
void frdm_kl25z_initialize_gpio(void)
{
    // enable clock for gpio led pins.
    SIM->SCGC5 |= SIM_SCGC5_PORTB(1);

    initialize_led_pin(LED_PIN_RED);
    initialize_led_pin(LED_PIN_GREEN);
    // toggle led pins
    GPIOB->PTOR |= (1 << LED_PIN_GREEN);

    log_item_t *item;
    BinaryLoggerStatus logger_status = BinaryLogger_OK;
    logger_status = CreateLogItem(&item);
    if (BinaryLogger_OK != logger_status) {
        abort();
    }
    logger_status = UpdateLogItem(item, GPIO_INITIALIZED, zero_payload_bytes,
                                  null_payload);
    if (BinaryLogger_OK != logger_status) {
        abort();
    }
    log_item(item);
    logger_status = DestroyLogItem(&item);
}

void frdm_kl25z_initialize_led_pin(GPIO_PINS led_pin)
{
    // Initialize the gpio pin for the led
    PORTB->PCR[led_pin] = PORT_PCR_MUX(1);
    // set pin to output
    GPIOB->PDDR |= (1 << led_pin);
    // initial state of pin is on
    GPIOB->PDOR |= (1 << led_pin);
}

void frdm_kl25z_update_leds(void)
{
    for (uint32_t i = 0; i < 200000; i++) {
        // do nothing for a while.
    }
    // toggle led pins
    GPIOB->PTOR |= (1 << LED_PIN_RED);
    GPIOB->PTOR |= (1 << LED_PIN_GREEN);
}

extern void UART0_IRQHandler(void)
{
    CircularBufferStatus cb_status = CB_No_Error;
    uint8_t byte;
    // What triggered the interrupt...
    if (UART0->S1 & UART0_S1_RDRF_MASK) {
        // received data register full
        byte = UART0->D;
        {
            // NOTE(bja, 2017-03) critical region accessing global data.
            cb_status = CircularBufferAddItem(logger.receive_buffer, &byte);
            global_async_data.data_available = true;
        }
        if (CB_No_Error == cb_status) {
            // do nothing? status flag is automatically reset
        } else {
            // error handling?
        }
    } else if (UART0->S1 & UART0_S1_TDRE_MASK) {
        // transmit data register empty
        {
            // NOTE(bja, 2017-03) critical region accessing global data.
            cb_status = CircularBufferRemoveItem(logger.transmit_buffer, &byte);
        }
        if (CB_No_Error == cb_status) {
            // successfully removed item.
            UART0->D = byte;
        } else {
            // nothing else to send
            UART0->C2 &= ~UART0_C2_TIE(1);
        }
    } else {
        // other interrupts to handle?
    }
}

#endif
