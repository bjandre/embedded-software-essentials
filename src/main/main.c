
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

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "platform-defs.h"
#include "logger.h"
#include "log-item.h"

#include "analyze-data.h"
#include "debug-uart-data.h"
#include "post.h"

#include "async-global.h"

// Global, asynchronously accessed data instances
volatile BinaryLogger_t logger;
volatile async_data_t global_async_data;

#if (PLATFORM == PLATFORM_FRDM)
#include "initialize-frdm-kl25z.h"
#include "interrupts-frdm-kl25z.h"
#include "gpio-frdm-kl25z.h"
#include "dma-frdm-kl25z.h"
#include "spi-frdm-kl25z.h"
#else
typedef enum GPIO_PINS {LED_PIN} GPIO_PINS;
#endif

/*
  Generic routine to initialize hardware.

  Wrapper around platform specific code.
 */
void initialize_hardware(void);

/*
  Generic routine to initialize GPIO.

  Wrapper around platform specific code.
 */
void initialize_interrupts(void);

/*
  Generic routine to initialize GPIO.

  Wrapper around platform specific code.
 */
void initialize_gpio(log_item_t *item);

/*
  Generic routine to initialize SPI.

  Wrapper around platform specific code.
 */
void initialize_spi(log_item_t *item);

/*
  Generic routine to initialize DMA.

  Wrapper around platform specific code.
 */
void initialize_dma(log_item_t *item);

/*
  Generic routine to update led status.

  Wrapper around platform specific code.
 */
void update_leds(void);


int main(int argc, char **argv)
{
    //PRINTF("Hello from Emebbed Software Essentials Project!\n");

    set_global_async_data_available(false);
    set_global_async_dma_complete(false);

    initialize_hardware();

    // NOTE(bja, 2017-03) logger uses interrupts on hardware where it is
    // supported. We need to enable them first to log the full initialization
    // sequence. They may need to be disabled selectively.
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
    logger_status = UpdateLogItemNoPayload(item, LOGGER_INITIALIZED);
    if (BinaryLogger_Success != logger_status) {
        abort();
    }
    log_item(item);

    initialize_gpio(item);
    initialize_dma(item);
    initialize_spi(item);

    UpdateLogItemNoPayload(item, SYSTEM_INITIALIZED);
    log_item(item);

    power_on_self_tests(item);

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

    UpdateLogItemNoPayload(item, DATA_ANALYSIS_STARTED);
    log_item(item);

    while (1) { /* main event loop */
        __asm("NOP"); /* breakpoint to stop while looping */
        update_leds();

#ifdef DEBUG_UART
        uint8_t tx_or_rx = 1;
        debug_uart(tx_or_rx, buffer, buffer_size);
#endif

#define DEBUG_SPI 1
#if DEBUG_SPI && (PLATFORM == PLATFORM_FRDM)
        // FIXME(bja, 2017-03) need to abstract out for host!
        frdm_kl25z_spi_transmit_byte(0x55);
#endif

#ifdef MOCK_RECEIVE_DATA_INTERRUPT
        set_global_async_data_available(true);
#endif

        data_available = get_global_async_data_available();
        set_global_async_data_available(false);

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
            UpdateLogItemNoPayload(item, DATA_ANALYSIS_COMPLETED);
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

void initialize_hardware(void)
{
#if (PLATFORM == PLATFORM_FRDM)
    frdm_kl25z_initialize();
#endif
}

void initialize_interrupts(void)
{
#if (PLATFORM == PLATFORM_FRDM)
    frdm_kl25z_initialize_interrupts();
#endif
}

void initialize_gpio(log_item_t *item)
{
#if (PLATFORM == PLATFORM_FRDM)
    frdm_kl25z_initialize_gpio();
#endif
    UpdateLogItemNoPayload(item, GPIO_INITIALIZED);
    log_item(item);
}

void initialize_spi(log_item_t *item)
{
#if (PLATFORM == PLATFORM_FRDM)
    frdm_kl25z_initialize_spi();
#endif
    UpdateLogItemNoPayload(item, SPI_INITIALIZED);
    log_item(item);
}

void initialize_dma(log_item_t *item)
{
#if (PLATFORM == PLATFORM_FRDM)
    frdm_kl25z_initialize_dma();
#endif
    UpdateLogItemNoPayload(item, DMA_INITIALIZED);
    log_item(item);
}

void update_leds(void)
{
#if (PLATFORM == PLATFORM_FRDM)
    frdm_kl25z_update_leds();
#endif
}

