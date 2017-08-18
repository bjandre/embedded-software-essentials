
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include <stdbool.h>
#include <time.h>

#include "MKL25Z4.h"
#include "system_MKL25Z4.h"

#include "rtc_frdm_kl25z.h"
#include "gpio_frdm_kl25z.h"

#include "async_global.h"

void frdm_kl25z_initialize_rtc(void)
{
    // enable the internal reference clock
    MCG->C1 |= MCG_C1_IRCLKEN(1);
    // use the slow internal reference clock
    MCG->C2 &= ~MCG_C2_IRCS(1);

    // setup clock output pin
    frdm_kl25z_initialize_port_c_output_pin(PTC_MCG_CLOCKOUT, 5);

    // set the internal reference clock as output clock
    SIM->SOPT2 |= SIM_SOPT2_CLKOUTSEL(4);

    // setup RTC clock input pin
    frdm_kl25z_initialize_port_c_input_pin(PTC_RTC_CLOCKIN, 1);

    // select RTC_CLKIN pin as 32kHz oscillator source
    SIM->SOPT1 |= SIM_SOPT1_OSC32KSEL(2);

    // enable the clock gate for the RTC module
    SIM->SCGC6 |= SIM_SCGC6_RTC(1);

    // initiate software reset of RTC
    RTC->CR |= RTC_CR_SWR(1);
    // clear the SWR bit
    RTC->CR &= ~RTC_CR_SWR(1);

    // enable the seconds interrupt
    RTC->IER |= RTC_IER_TSIE(1);
    NVIC_EnableIRQ(RTC_Seconds_IRQn);

    // set the time seconds register with the compile time epoch
    RTC->TSR = EPOCH_TIME;
    set_global_async_heartbeat_timestamp((time_t)(RTC->TSR));

    // start counting
    RTC->SR |= RTC_SR_TCE(1);
}

extern void RTC_Seconds_IRQHandler(void)
{
    // NOTE: no status flag to clear
    set_global_async_heartbeat_timestamp((time_t)(RTC->TSR));
    set_global_async_heartbeat_occurred(true);
}
