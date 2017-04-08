
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
#define TESTING_MOCK_INTERRUPT
#define PRINTF(ARGS) printf(ARGS)
#include <stdio.h>
#endif

#include <stdint.h>
#include <stdlib.h>

#include "platform-defs.h"

#include "hal.h"

#include "logger.h"
#include "log-item.h"

#include "analyze-data.h"
#include "debug-uart-data.h"
#include "post.h"
#include "heartbeat.h"
#include "profiler.h"

#include "async-global.h"

// Global, asynchronously accessed data instances
volatile async_data_t global_async_data;

#if (PLATFORM == PLATFORM_FRDM)
#include "gpio-frdm-kl25z.h"
#include "spi-frdm-kl25z.h"
#else
typedef enum GPIO_PINS {LED_PIN} GPIO_PINS;
#endif

/**
  Initialization sequence

  \param[out] item pointer to log item, should be null initially, a valid log
  item will be created during initialization and returned.
; */
void initialize(log_item_t **item);

/**
  Generic routine to update led status.

  Wrapper around platform specific code.
 */
void update_leds();


int main(int argc, char **argv)
{
    /*
      system initialization
    */

    initialize_global_async_data();

#ifdef TESTING_MOCK_INTERRUPT
    uint32_t testing_timestamp = 1491352432U;
    set_global_async_heartbeat_timestamp(testing_timestamp);
#endif

    log_item_t *item = NULL;

    initialize(&item);

    initialize_profiler();

    power_on_self_tests(item);

    /*
      application initialization
     */
#undef DEBUG_UART
#ifdef DEBUG_UART
    size_t const buffer_size = 32 * sizeof(uint8_t);
    uint8_t *buffer = malloc(buffer_size);
#endif

    data_summary_t data_summary;
    initialize_logger_data_analysis(&data_summary, item);

#ifdef TESTING_MOCK_INTERRUPT
    set_global_async_heartbeat_occurred(true);
    testing_timestamp++;
    set_global_async_heartbeat_timestamp(testing_timestamp);
#endif

    while (1) { /* main event loop */
        __asm("NOP"); /* breakpoint to stop while looping */
        heartbeat(item);

#ifdef DEBUG_UART
        uint8_t tx_or_rx = 1;
        debug_uart(tx_or_rx, buffer, buffer_size);
#endif

#define DEBUG_SPI 1
#if DEBUG_SPI && (PLATFORM == PLATFORM_FRDM)
        // FIXME(bja, 2017-03) need to abstract out for host!
        frdm_kl25z_toggle_green_led();
        frdm_kl25z_spi_transmit_byte(0x55);
#endif

        analyze_logger_data_event(&data_summary, item);

    }
#ifdef DEBUG_UART
    free(buffer);
#endif
    DestroyLogItem(&item);
    return 0;
}

void initialize(log_item_t **item)
{
    initialize_hardware();

    BinaryLoggerStatus logger_status = BinaryLogger_Success;
    uint8_t const buffer_size_bytes = 32;
    logger_status = BinaryLoggerCreate(buffer_size_bytes);
    if (BinaryLogger_Success != logger_status) {
        abort();
    }

    InitializeLoggerForLogItems();
    logger_status = CreateLogItem(item);
    if (BinaryLogger_Success != logger_status) {
        abort();
    }
    logger_status = UpdateLogItemNoPayload(*item, LOGGER_INITIALIZED);
    if (BinaryLogger_Success != logger_status) {
        abort();
    }
    log_item(*item);

    initialize_gpio();
    UpdateLogItemNoPayload(*item, GPIO_INITIALIZED);
    log_item(*item);

    initialize_rtc();

    initialize_dma();
    UpdateLogItemNoPayload(*item, DMA_INITIALIZED);
    log_item(*item);

    initialize_spi();
    UpdateLogItemNoPayload(*item, SPI_INITIALIZED);
    log_item(*item);

    UpdateLogItemNoPayload(*item, SYSTEM_INITIALIZED);
    log_item(*item);

}

void update_leds(void)
{
#if (PLATFORM == PLATFORM_FRDM)
    frdm_kl25z_toggle_red_led();
#endif
}

