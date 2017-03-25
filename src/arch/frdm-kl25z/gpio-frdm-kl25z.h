
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef ESE_ARCH_GPIO_FRDM_KL25Z_H_
#define ESE_ARCH_GPIO_FRDM_KL25Z_H_

/**
   \file gpio-frdm-kl25z.c
   platform specific code for the frdm-kl25z
*/

/**
   define the pins for the leds.
*/
typedef enum GPIO_PINS {
    LED_PIN_RED = 18,
    LED_PIN_GREEN = 19,
} GPIO_PINS;

/**
  Platform specific GPIO initialization
 */
void frdm_kl25z_initialize_gpio(void);

/**
  Platform specific code to initialize leds.

  \param[in] led_pin gpio pin for the led

 */
void frdm_kl25z_initialize_led_pin(GPIO_PINS led_pin);

/**
  Platform specific code to update led state
 */
void frdm_kl25z_update_leds(void);


#endif // ESE_ARCH_GPIO_FRDM_KL25Z_H_
