/**
   \file gpio_bbb.h

   Public interface to HAL for Beaglebone Black GPIO
 */

/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef ESE_HAL_BBB_GPIO_H_
#define ESE_HAL_BBB_GPIO_H_

#include <stdint.h>

#include "gpio_common.h"

/**
   Initialize hardware for gpio

   \return status
 */
gpio_status_t gpio_bbb_initialize(void);

/**
   Shutdown all gpio pins

   \return status
 */
gpio_status_t gpio_bbb_shutdown(void);

/**
   Initialize the specified GPIO pin for input or output.

   \param[in] pin_number - linux gpio pin number
   \param[in] direction - input or output

   \return status
 */
gpio_status_t gpio_bbb_initialize_pin(const gpio_pin_num_t pin_number,
                                      const gpio_pin_direction_t direction);

/**
   Shutdown the specified GPIO pin, including freeing all dynamically created
   memory.

   \param[in] pin_number - linux gpio pin number
   \param[in] direction - input or output

   \return status
 */
gpio_status_t gpio_bbb_shutdown_pin(const gpio_pin_num_t pin_number);

/**
   Set the specified GPIO pin to high / 1 / on
   memory.

   \param[in] pin_number - linux gpio pin number

   \return status
 */
gpio_status_t gpio_bbb_set_pin(const gpio_pin_num_t pin_number);

/**
   Clear the specified GPIO pin to low / 0 / off
   memory.

   \param[in] pin_number - linux gpio pin number

   \return status
 */
gpio_status_t gpio_bbb_clear_pin(const gpio_pin_num_t pin_number);


#endif /* ESE_HAL_BBB_GPIO_H_ */
