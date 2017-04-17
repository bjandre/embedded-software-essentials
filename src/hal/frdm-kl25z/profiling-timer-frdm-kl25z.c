
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include <math.h>
#include <stdint.h>

#include "MKL25Z4.h"
#include "system_MKL25Z4.h"

#include "math-util.h"
#include "async-global.h"

static uint32_t clocks_per_sec;

/**
   Set the clocks per second for this platform.
 */
void frdm_kl25z_set_clocks_per_second_profiling_timer(uint32_t cps);

/**
   Determine the prescaler selection factor.

   \param prescaler desired prescaler

   \return required selection factor for the prescaler
 */
uint8_t determine_prescaler_selection(uint32_t prescaler);


uint32_t frdm_kl25z_get_clocks_per_second_profiling_timer(void)
{
    return clocks_per_sec;
}


void frdm_kl25z_set_clocks_per_second_profiling_timer(uint32_t cps)
{
    clocks_per_sec = cps;
}

void frdm_kl25z_initialize_profiling_timer(void)
{
    // setup TPM0 as the timer used for profiling. It is just a high frequency,
    // free running counter.

    // NOTE(bja, 2017-03) TPM0->CNT is a 16 bit counter

    // enable the clock gate for timer 0
    SIM->SCGC6 |= SIM_SCGC6_TPM0(1);
    // clock source for all TPM is MCGFLLCLK clock or MCGPLLCLK
    SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);

    // increment every counter clock
    TPM0->SC |= TPM_SC_CMOD(1);

    // prescaler: Clock / Scaler = Timer; Clock / Timer = Scaler
    // uint32_t timer_resolution = 1000000u; // 1 us
    // uint32_t prescaler = SystemCoreClock / timer_resolution;
    // prescaler = next_power_of_2(prescaler);

    // want to report clock ticks, not seconds, so just scale as 1;
    uint32_t prescaler = 1;

    uint8_t prescaler_selection = determine_prescaler_selection(prescaler);
    TPM0->SC |= TPM_SC_PS(prescaler_selection);

    // FIXME(bja, 2017-04) don't hard code! Since we are incrementing the
    // counter every clock tic, and using a prescaler of 1, we can just set
    // clocks_per_sec to the clock value.
    frdm_kl25z_set_clocks_per_second_profiling_timer(SystemCoreClock);

    // continue counting in debug mode?

    // up counting only
    TPM0->SC &= ~TPM_SC_CPWMS(1);

    // upper limit of counter, all available 16 bits
    TPM0->MOD = 0xFFFFU;

    // initial value
    TPM0->CNT = 0;

    // enable the timer overflow interrupt
    TPM0->SC |= TPM_SC_TOIE(1);
    NVIC_EnableIRQ(TPM0_IRQn);
}

uint8_t determine_prescaler_selection(uint32_t prescaler)
{
    uint8_t prescaler_selection = 0;
    switch (prescaler) {
    case 1:
        prescaler_selection = 0;
        break;
    case 2:
        prescaler_selection = 1;
        break;
    case 4:
        prescaler_selection = 2;
        break;
    case 8:
        prescaler_selection = 3;
        break;
    case 16:
        prescaler_selection = 4;
        break;
    case 32:
        prescaler_selection = 5;
        break;
    case 64:
        prescaler_selection = 6;
        break;
    case 128:
        prescaler_selection = 7;
        break;
    default:
        break;
    }
    return prescaler_selection;
}

extern void TPM0_IRQHandler(void)
{
    // check TPM0->TOF, Timer Overflow Flag, to determine if
    // overflow occurred. write 1 to clear. If it is still set, then multiple
    // overflows occurred.
    if (TPM0->SC & TPM_SC_TOF(1)) {
        // increment global overflow counter
        increment_global_async_profiling_overflow();
        // clear flag
        TPM0->SC |= TPM_SC_TOF(1);
    }
}
