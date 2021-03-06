
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include "MKL25Z4.h"

#include "gpio_frdm_kl25z.h"

/**
   \file gpio_frdm_kl25z.c

   Platform specific GPIO code for the frdm-kl25z

*/
void frdm_kl25z_initialize_gpio(void)
{
    // enable clock for gpio for red and green led pins.
    SIM->SCGC5 |= SIM_SCGC5_PORTB(1);
    // enable clock for gpio for blue led pin.
    SIM->SCGC5 |= SIM_SCGC5_PORTD(1);

    frdm_kl25z_initialize_port_b_output_pin(PTB_GPIO_LED_RED);
    frdm_kl25z_initialize_port_b_output_pin(PTB_GPIO_LED_GREEN);
    frdm_kl25z_initialize_port_d_output_pin(PTD_GPIO_LED_BLUE);

    // initial state of pins
    frdm_kl25z_toggle_red_led();
    frdm_kl25z_toggle_green_led();
    GPIOD->PTOR |= (1 << PTD_GPIO_LED_BLUE);
}

void frdm_kl25z_initialize_port_b_output_pin(gpio_pin_num_t pin)
{
    // Initialize the gpio pin for desired function
    // PORTB->PCR[pin] |= PORT_PCR_MUX(1);
    // Initialize the link to TPM2 channel
    PORTB->PCR[pin] |= PORT_PCR_MUX(3);
    // set pin to output
    GPIOB->PDDR |= (1 << pin);
}

void frdm_kl25z_initialize_port_c_output_pin(gpio_pin_num_t pin, uint8_t mux)
{
    // enable clock for port C
    SIM->SCGC5 |= SIM_SCGC5_PORTC(1);
    // Initialize the gpio pin for desired function
    PORTC->PCR[pin] |= PORT_PCR_MUX(mux);
    // set pin to output
    GPIOC->PDDR |= (1 << pin);
}

void frdm_kl25z_initialize_port_c_input_pin(gpio_pin_num_t pin, uint8_t mux)
{
    // enable clock for port C
    SIM->SCGC5 |= SIM_SCGC5_PORTC(1);
    // Initialize the gpio pin for desired function
    PORTC->PCR[pin] |= PORT_PCR_MUX(mux);
    // set pin to output
    GPIOC->PDDR &= ~(1 << pin);
}

void frdm_kl25z_initialize_port_d_output_pin(gpio_pin_num_t pin)
{
    // Initialize the gpio pin desired function
    PORTD->PCR[pin] |= PORT_PCR_MUX(1);
    // set pin to output
    GPIOD->PDDR |= (1 << pin);
}

void frdm_kl25z_toggle_red_led(void)
{
    GPIOB->PTOR |= (1 << PTB_GPIO_LED_RED);
}

void frdm_kl25z_toggle_green_led(void)
{
    GPIOB->PTOR |= (1 << PTB_GPIO_LED_GREEN);
}

void frdm_kl25z_heartbeat_led(void)
{
    GPIOD->PTOR |= (1 << PTD_GPIO_LED_BLUE);
}
