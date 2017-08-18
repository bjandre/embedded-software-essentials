
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include "MKL25Z4.h"

#include "initialize_frdm_kl25z.h"

void frdm_kl25z_initialize(void)
{
    // PLL/FLL clock select - Selects the MCGPLLCLK or MCGFLLCLK clock for
    // various peripheral clocking options.
    SIM->SOPT2 &= ~SIM_SOPT2_PLLFLLSEL(1);

    // set the 32K oscillator source to the system clock
    SIM->SOPT1 &= ~SIM_SOPT1_OSC32KSEL(3);
    // set the RTC clock output to the RTC 1 Hz
    SIM->SOPT2 &= ~SIM_SOPT2_RTCCLKOUTSEL(1);
}

