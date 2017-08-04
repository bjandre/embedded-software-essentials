
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

#include "platform-defs.h"

#include "hal.h"
#include "spi-peripheral-factory.h"

#include "logger.h"
#include "log-item.h"
#include "message.h"
#include "command.h"

#include "analyze-data.h"
#include "debug-uart-data.h"
#include "post.h"
#include "heartbeat.h"
#include "profiler.h"
#include "memory-profile.h"

#include "async-global.h"

/* Global, asynchronously accessed data instances */
volatile async_data_t global_async_data;

#if (PLATFORM == PLATFORM_FRDM)
#include "gpio-frdm-kl25z.h"
#include "spi-frdm-kl25z.h"
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

    initialize_rtc();
    UpdateLogItemNoPayload(*item, RTC_INITIALIZED);
    log_item(*item);

    initialize_gpio();
    UpdateLogItemNoPayload(*item, GPIO_INITIALIZED);
    log_item(*item);

    initialize_dma();
    UpdateLogItemNoPayload(*item, DMA_INITIALIZED);
    log_item(*item);

    initialize_spi();
    UpdateLogItemNoPayload(*item, SPI_INITIALIZED);
    log_item(*item);

    UpdateLogItemNoPayload(*item, SYSTEM_INITIALIZED);
    log_item(*item);

}

void shutdown(log_item_t **item)
{
    UpdateLogItemNoPayload(*item, SYSTEM_HALTED);
    log_item(*item);
    DestroyLogItem(item);
    log_flush();
    BinaryLoggerDestroy();
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

