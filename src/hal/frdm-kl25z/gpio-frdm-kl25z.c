
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include "MKL25Z4.h"

#include "gpio-frdm-kl25z.h"

/**
   \file gpio-frdm-kl25z.c

   Platform specific GPIO code for the frdm-kl25z

*/
void frdm_kl25z_initialize_gpio(void)
{
    // enable clock for gpio for red and green led pins.
    SIM->SCGC5 |= SIM_SCGC5_PORTB(1);
    // enable clock for gpio for blue led pin.
    SIM->SCGC5 |= SIM_SCGC5_PORTD(1);

    frdm_kl25z_initialize_port_b_output_pin(LED_PIN_RED);
    frdm_kl25z_initialize_port_b_output_pin(LED_PIN_GREEN);
    frdm_kl25z_initialize_port_d_output_pin(LED_PIN_BLUE);

    // initial state of pins
    GPIOB->PCOR |= (1 << LED_PIN_RED);
    GPIOB->PCOR |= (1 << LED_PIN_GREEN);
    GPIOD->PSOR |= (1 << LED_PIN_BLUE);
}

void frdm_kl25z_initialize_port_b_output_pin(GPIO_PINS pin)
{
    // Initialize the gpio pin for the led
    PORTB->PCR[pin] = PORT_PCR_MUX(1);
    // set pin to output
    GPIOB->PDDR |= (1 << pin);
}

void frdm_kl25z_initialize_port_d_output_pin(GPIO_PINS pin)
{
    // Initialize the gpio pin for the led
    PORTD->PCR[pin] = PORT_PCR_MUX(1);
    // set pin to output
    GPIOD->PDDR |= (1 << pin);
}

void frdm_kl25z_update_leds(void)
{
    //for (uint32_t i = 0; i < 200000; i++) {
    // do nothing for a while.
    //}
    // toggle led pins
    GPIOB->PTOR |= (1 << LED_PIN_RED);
    GPIOB->PTOR |= (1 << LED_PIN_GREEN);
}

void frdm_kl25z_blue_led_on(void)
{
    GPIOD->PCOR |= (1 << LED_PIN_BLUE);
}
