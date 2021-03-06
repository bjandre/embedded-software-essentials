
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

#include "math_util.h"
#include "async_global.h"

/**
   Determine the prescaler selection factor.

   \param prescaler desired prescaler

   \return required selection factor for the prescaler
 */
uint8_t pwm_determine_prescaler_selection(uint32_t prescaler);


void frdm_kl25z_initialize_pwm_timer(void)
{
    // setup TPM2 as the timer used for pwm. It is just a high frequency,
    // free running counter.

    // NOTE(bja, 2017-03) TPM2->CNT is a 16 bit counter

    // enable the clock gate for timer 0
    SIM->SCGC6 |= SIM_SCGC6_TPM2(1);
    // clock source for all TPM is MCGFLLCLK clock or MCGPLLCLK
    SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);

    // increment every counter clock
    TPM2->SC |= TPM_SC_CMOD(1);

    // prescaler: Clock / Scaler = Timer; Clock / Timer = Scaler
    // uint32_t timer_resolution = 1000000u; // 1 us
    // uint32_t prescaler = SystemCoreClock / timer_resolution;
    // prescaler = next_power_of_2(prescaler);

    // want to report clock ticks, not seconds, so just scale as 1;
    uint32_t prescaler = 1;

    uint8_t prescaler_selection = pwm_determine_prescaler_selection(prescaler);
    TPM2->SC |= TPM_SC_PS(prescaler_selection);

    // continue counting in debug mode?

    // up counting only
    TPM2->SC |= TPM_SC_CPWMS(1);

    // upper limit of counter, all available 16 bits - 1 so we can exceed the
    // value and disable the PWM on a channel
    TPM2->MOD = 0xFFFEu;

    // initial value
    TPM2->CNT = 0;

    // initialize the channels for center aligned PWM
    TPM2->CONTROLS[0].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;
    TPM2->CONTROLS[0].CnV = 0;
    TPM2->CONTROLS[1].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;
    TPM2->CONTROLS[1].CnV = 0;
}

uint8_t pwm_determine_prescaler_selection(uint32_t prescaler)
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
