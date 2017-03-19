
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
#include "log-item.h"

#include "analyze-data.h"
#include "debug-uart-data.h"

#include "async-global.h"

// Global, asynchronously accessed data instances
volatile BinaryLogger_t logger;
volatile async_data_t global_async_data;

#if (PLATFORM == PLATFORM_FRDM)
#include "interrupts-frdm-kl25z.h"
#include "gpio-frdm-kl25z.h"
#else
typedef enum GPIO_PINS {LED_PIN} GPIO_PINS;
#endif
/*
  Generic routine to initialize GPIO.

  Wrapper around platform specific code.
 */
void initialize_interrupts(void);

/*
  Generic routine to initialize GPIO.

  Wrapper around platform specific code.
 */
void initialize_gpio(void);

/*
  Generic routine to initialize an led pin.

  Wrapper around platform specific code.

  Parameters: led pin enumeration
 */
void initialize_led_pin(GPIO_PINS led_pin);

/*
  Generic routine to update led status.

  Wrapper around platform specific code.
 */
void update_leds(void);


int main(int argc, char **argv)
{
    //PRINTF("Hello from Emebbed Software Essentials Project!\n");

    global_async_data.data_available = false;

    initialize_interrupts();

    BinaryLoggerStatus logger_status = BinaryLogger_Success;
    uint8_t const buffer_size_bytes = 32;
    logger_status = BinaryLoggerCreate(buffer_size_bytes);
    if (BinaryLogger_Success != logger_status) {
        abort();
    }

    InitializeLoggerForLogItems();
    log_item_t *item;
    logger_status = CreateLogItem(&item);
    if (BinaryLogger_Success != logger_status) {
        abort();
    }
    logger_status = UpdateLogItem(item, LOGGER_INITIALIZED, zero_payload_bytes,
                                  null_payload);
    if (BinaryLogger_Success != logger_status) {
        abort();
    }
    log_item(item);

    initialize_gpio();

    logger_status = UpdateLogItem(item, SYSTEM_INITIALIZED, zero_payload_bytes,
                                  null_payload);
    if (BinaryLogger_Success != logger_status) {
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
    if (BinaryLogger_Success != logger_status) {
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
            if (BinaryLogger_Success != logger_status) {
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
            if (BinaryLogger_Success != logger_status) {
                abort();
            }
            log_item(item);
            clear_data_summary(&data_summary);
            num_received = 0;
#ifdef MOCK_RECEIVE_DATA_INTERRUPT
            exit(EXIT_SUCCESS);
#endif
        }
    }
#ifdef DEBUG_UART
    free(buffer);
#endif
    logger_status = DestroyLogItem(&item);
    return 0;
}

void initialize_interrupts(void)
{
#if (PLATFORM == PLATFORM_FRDM)
    frdm_kl25z_initialize_interrupts();
#endif
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

