
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include "MKL25Z4.h"
#include "profiler.h"
#include "log-item.h"

static uint32_t total_latency;
static profiling_timer_data_t start_time;
static const uint32_t pin = 10;

void interrupt_latency_frdm_kl25z(log_item_t *item)
{
    // Assume port d clock is already enabled
    // trigger an interrupt via a port
    PORTD->PCR[pin] |= PORT_PCR_MUX(1);
    // interrupt when logic 1, 1100
    PORTD->PCR[pin] |= PORT_PCR_IRQC(12);
    PORTD->ISFR |= (1 << pin);
    // set pin to output
    GPIOD->PDDR |= (1 << pin);
    // clear the pin
    GPIOD->PCOR |= (1 << pin);
    NVIC_EnableIRQ(PORTD_IRQn);

    uint8_t num_tests = 100;
    total_latency = 0;
    // directly trigger an interrupt
    for (uint8_t n = 0; n < num_tests; n++) {
        get_timer(&start_time);
        NVIC_SetPendingIRQ(PORTD_IRQn);
    }
    uint32_t average_latency = total_latency / num_tests;
    UpdateLogItem(item, PROFILING, sizeof(average_latency), &average_latency);
    log_item(item);
    log_flush();

    total_latency = 0;
    for (uint8_t n = 0; n < num_tests; n++) {
        get_timer(&start_time);
        // set the pin high to trigger an interrupt
        GPIOD->PSOR |= (1 << pin);
    }
    average_latency = total_latency / num_tests;
    UpdateLogItem(item, PROFILING, sizeof(average_latency), &average_latency);
    log_item(item);
    log_flush();
}


extern void PORTD_IRQHandler(void)
{
    total_latency += elapsed_time(&start_time);
    // set the pin low
    GPIOD->PCOR |= (1 << pin);
    // clear the interrupt
    PORTD->PCR[pin] |= PORT_PCR_ISF(1);
}
