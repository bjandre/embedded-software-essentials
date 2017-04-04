
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef ESE_HAL_GPIO_FRDM_KL25Z_H_
#define ESE_HAL_GPIO_FRDM_KL25Z_H_

#include <stdint.h>

/**
   \file gpio-frdm-kl25z.c
   platform specific code for the frdm-kl25z
*/

/**
   define the pins used for various gpio tasks
*/
typedef uint8_t GPIO_PINS;

static const GPIO_PINS PTB_GPIO_LED_RED = 18; //!< built in red led, port b
static const GPIO_PINS PTB_GPIO_LED_GREEN = 19; //!< built in green led, port b

static const GPIO_PINS PTD_GPIO_LED_BLUE = 1; //!< built in blue led, port d
static const GPIO_PINS PTD_SPI1_CS =
    4; //!< Alternate function 2 - SPI1_PCS0 - slave select
static const GPIO_PINS PTD_SPI1_CLK =
    5; //!< Alternate function 2 - SPI1_SCK - clock
static const GPIO_PINS PTD_SPI1_MOSI =
    6; //!< Alternate function 2 - SPI1_MOSI - Master Out Slave In
static const GPIO_PINS PTD_SPI1_MISO =
    7; //!< Alternate function 2 - SPI1_MISO - Master In Slave Out


/**
  Platform specific GPIO initialization
 */
void frdm_kl25z_initialize_gpio(void);

/**
  Platform specific code to initialize GPIO pins as output on port b.

  \param[in] pin gpio output pin

 */
void frdm_kl25z_initialize_port_b_output_pin(GPIO_PINS pin);

/**
  Platform specific code to initialize GPIO pins as output on port d.

  \param[in] pin gpio output pin

 */
void frdm_kl25z_initialize_port_d_output_pin(GPIO_PINS pin);

/**
  Platform specific code to toggle the red led
 */
void frdm_kl25z_toggle_red_led(void);

/**
  Platform specific code to toggle the green led
 */
void frdm_kl25z_toggle_green_led(void);

/**
   Platform specific code to turn on the blue LED
*/
void frdm_kl25z_blue_led_on(void);


#endif // ESE_HAL_GPIO_FRDM_KL25Z_H_
