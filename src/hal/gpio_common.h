
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef ESE_HAL_GPIO_COMMON_H_
#define ESE_HAL_GPIO_COMMON_H_

#include "platform_defs.h"

typedef enum {
    GPIO_INPUT,
    GPIO_OUTPUT,
} gpio_pin_direction_t;

typedef enum {
    GPIO_ERROR,
    GPIO_SUCCESS,
} gpio_status_t;

typedef uint8_t gpio_pin_num_t;
#if (PLATFORM == PLATFORM_FRDM)
static const gpio_pin_num_t NRF24_CHIP_ACTIVATE = 3;
#elif (PLATFORM == PLATFORM_BBB)
static const gpio_pin_num_t NRF24_CHIP_ACTIVATE = 48;
#else
static const gpio_pin_num_t NRF24_CHIP_ACTIVATE = 0;
#endif

#endif/* ESE_HAL_GPIO_COMMON_H_ */
