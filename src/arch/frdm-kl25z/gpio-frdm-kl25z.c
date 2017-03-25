
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include <stdint.h>
#include <stdlib.h>

#include "MKL25Z4.h"

#include "gpio-frdm-kl25z.h"

#include "logger.h"
#include "log-item.h"

/**
   \file gpio-frdm-kl25z.c
   platform specific code for the frdm-kl25z
*/
void frdm_kl25z_initialize_gpio(void)
{
    // enable clock for gpio led pins.
    SIM->SCGC5 |= SIM_SCGC5_PORTB(1);

    frdm_kl25z_initialize_led_pin(LED_PIN_RED);
    frdm_kl25z_initialize_led_pin(LED_PIN_GREEN);
    // toggle led pins
    GPIOB->PTOR |= (1 << LED_PIN_GREEN);
}

void frdm_kl25z_initialize_led_pin(GPIO_PINS led_pin)
{
    // Initialize the gpio pin for the led
    PORTB->PCR[led_pin] = PORT_PCR_MUX(1);
    // set pin to output
    GPIOB->PDDR |= (1 << led_pin);
    // initial state of pin is on
    GPIOB->PDOR |= (1 << led_pin);
}

void frdm_kl25z_update_leds(void)
{
    for (uint32_t i = 0; i < 200000; i++) {
        // do nothing for a while.
    }
    // toggle led pins
    GPIOB->PTOR |= (1 << LED_PIN_RED);
    GPIOB->PTOR |= (1 << LED_PIN_GREEN);
}

