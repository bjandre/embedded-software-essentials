
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

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

#include "platform_defs.h"

#include "hal.h"
#include "spi_peripheral_factory.h"

#include "binary_logger.h"
#include "log_item.h"
#include "message.h"
#include "command.h"

#include "analyze_data.h"
#include "debug_uart_data.h"
#include "post.h"
#include "heartbeat.h"
#include "profiler.h"
#include "memory_profile.h"

#include "async_global.h"

/* Global, asynchronously accessed data instances */
volatile async_data_t global_async_data;

#if (PLATFORM == PLATFORM_FRDM)
#include "gpio_frdm_kl25z.h"
#include "spi_frdm_kl25z.h"
#include "nrf24l01.h"
#elif (PLATFORM == PLATFORM_BBB)
#include "nrf24l01.h"
#endif

/**
  Initialization sequence

  \param[out] item pointer to log item, should be null initially, a valid log
  item will be created during initialization and returned.
; */
void initialize(log_item_t **item);

/**
  Run the shutdown sequence : send final log message, free global memory,
  etc. Mostly for host and embedded linux systems where we can run memory leak
  detectors.

  \param[in] **item pointer to log item that will be freed
 */
void shutdown(log_item_t **item);

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

#if (PLATFORM == PLATFORM_FRDM) || (PLATFORM == PLATFORM_BBB)
    /* FIXME(bja, 2017-03) need to abstract out for host! */
    size_t num_bytes_buffer = 128;
    nrf24_initialize(num_bytes_buffer);
#endif

    power_on_self_tests(item);

    /*
      application initialization
     */
    initialize_command_interpreter();

#ifdef TESTING_MOCK_INTERRUPT
    set_global_async_heartbeat_occurred(true);
    testing_timestamp++;
    set_global_async_heartbeat_timestamp(testing_timestamp);
#endif

    message_t message;
    reset_message(&message);

    while (1) { /* main event loop */
        __asm("NOP"); /* breakpoint to stop while looping */
        heartbeat(item);
        process_message(item, &message);
        process_commands(item);

        bool software_reset = get_global_async_software_reset();
        if (software_reset) {
            break;
        }
    }
    shutdown(&item);
    return 0;
}

void initialize(log_item_t **item)
{
    initialize_hardware();

    binary_logger_status_t logger_status = BINARY_LOGGER_SUCCESS;
    uint8_t const buffer_size_bytes = 32;
    logger_status = binary_logger_create(buffer_size_bytes);
    if (BINARY_LOGGER_SUCCESS != logger_status) {
        abort();
    }

    log_item_initialize_logger();
    logger_status = log_item_create(item);
    if (BINARY_LOGGER_SUCCESS != logger_status) {
        abort();
    }
    logger_status = log_item_update_no_payload(*item, LOGGER_INITIALIZED);
    if (BINARY_LOGGER_SUCCESS != logger_status) {
        abort();
    }
    log_item(*item);

    initialize_rtc();
    log_item_update_no_payload(*item, RTC_INITIALIZED);
    log_item(*item);

    initialize_gpio();
    log_item_update_no_payload(*item, GPIO_INITIALIZED);
    log_item(*item);

    initialize_dma();
    log_item_update_no_payload(*item, DMA_INITIALIZED);
    log_item(*item);

    initialize_spi();
    log_item_update_no_payload(*item, SPI_INITIALIZED);
    log_item(*item);

    log_item_update_no_payload(*item, SYSTEM_INITIALIZED);
    log_item(*item);

}

void shutdown(log_item_t **item)
{
    log_item_update_no_payload(*item, SYSTEM_HALTED);
    log_item(*item);
    log_item_destroy(item);
    binary_logger_flush();
    binary_logger_destroy();
#if (PLATFORM == PLATFORM_FRDM)
    NVIC_SystemReset();
#endif
}

void update_leds(void)
{
#if (PLATFORM == PLATFORM_FRDM)
    frdm_kl25z_toggle_red_led();
#endif
}

