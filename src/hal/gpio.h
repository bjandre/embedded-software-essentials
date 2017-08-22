/**
   \file gpio.h

   Public interface of the HAL for GPIO
 */

/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef ESE_HAL_GPIO_H_
#define ESE_HAL_GPIO_H_

#include <stdint.h>

#include "gpio_common.h"

/**
   Activate a GPIO pin - set to one

   \param[in] pin Id
 */
void gpio_set_pin(gpio_pin_num_t const pin);

/**
   Clear a GPIO pin - set to zero

   \param[in] pin id
 */
void gpio_clear_pin(gpio_pin_num_t const pin);

/**
   Initialize the chip active pin used by the nrf24l01

 */
void gpio_initialize_chip_active_pin(void);

/**
   Toggle the heartbeat led

 */
void gpio_toggle_heartbeat_led(void);

/**
   Toggle the red led
 */
void gpio_toggle_red_led(void);

/**
   Toggle the green led
 */
void gpio_toggle_green_led(void);

/**
   Toggle an led controlled by PWM

   \param[in] pin id
 */
void gpio_pwm_toggle_led(uint32_t const index);

/**
   Increase the intensity of an led controlled by PWM

   \param[in] pin id
 */
void gpio_pwm_increase_intensity(uint32_t const index);

/**
   Decrease the intensity of an led controlled by PWM

   \param[in] pind id
 */
void gpio_pwm_decrease_intensity(uint32_t const index);


#endif /* ESE_HAL_GPIO_H_ */
