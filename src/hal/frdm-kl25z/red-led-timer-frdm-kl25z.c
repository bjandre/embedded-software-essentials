
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


/**
   \file red-led-timer-frdm-kl25z.c

   Platform specific GPIO/TPM code for blinking the red led on the frdm-kl25z

*/

static const uint32_t PTB_GPIO_LED_RED = 18; //!< built in red led, port b

/**
   initialize gpio for the red pin
 */
void frdm_kl25z_initialize_red_led_pin(void)
{
    // enable clock for gpio for red led on port b
    SIM->SCGC5 |= SIM_SCGC5_PORTB(1);
    // Initialize the gpio pin for aux 1, general gpio control
    PORTB->PCR[PTB_GPIO_LED_RED] |= PORT_PCR_MUX(1);
    // set pin to output
    GPIOB->PDDR |= (1 << PTB_GPIO_LED_RED);
    // initial state of pin is off
    GPIOD->PCOR |= (1 << PTB_GPIO_LED_RED);
}

/**
   initialize the timer to control the red pin
 */
void frdm_kl25z_initialize_red_led_timer(void)
{
    // setup TPM1 as the timer for toggling the led.

    // TPM1->CNT is a 16 bit counter

    // enable the clock gate for timer 1
    SIM->SCGC6 |= SIM_SCGC6_TPM1(1);
    // clock source for all TPM is MCGFLLCLK clock or MCGPLLCLK
    SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);

    // increment every counter clock
    TPM1->SC |= TPM_SC_CMOD(1);

    // prescaler: Clock / Scaler = Timer frequency;
    // since we are dealing with milliseconds, assume a scaler of 128
    // Timer frequency = 20971520 Hz / 128 = 163840 Hz
    // timer_resolution = 0.05 seconds
    // Interrupt ticks = timer frequency * timer resolution
    // Interrupt ticks = 163840 ticks / seconds * 0.05 seconds = 8192.0
    uint8_t prescaler_selection = 111;
    TPM1->SC |= TPM_SC_PS(prescaler_selection);

    // up counting only
    TPM1->SC &= ~TPM_SC_CPWMS(1);

    // upper limit of counter, calculated above
    TPM1->MOD = 0x2000;

    // initial value
    TPM1->CNT = 0;

    // enable the timer overflow interrupt
    TPM1->SC |= TPM_SC_TOIE(1);
    NVIC_EnableIRQ(TPM1_IRQn);
}

/**
   timer irq handler to toggle the red pin
 */
extern void TPM1_IRQHandler(void)
{
    // check TPM0->TOF, Timer Overflow Flag, to determine if
    // overflow occurred. write 1 to clear. If it is still set, then multiple
    // overflows occurred.
    if (TPM1->SC & TPM_SC_TOF(1)) {
        // toggle led
        GPIOB->PTOR |= (1 << PTB_GPIO_LED_RED);
        // clear interrupt flag
        TPM1->SC |= TPM_SC_TOF(1);
    }
}
