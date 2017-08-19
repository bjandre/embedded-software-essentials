/**
   \file leds.h

   Public interface for application control LEDs
 */

/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef ESE_APP_LEDS_H_
#define ESE_APP_LEDS_H_

#include <stdint.h>

static const uint8_t increase_intensity = 0x01;
static const uint8_t decrease_intensity = 0x02;

typedef enum {
    LEDS_RED,
    LEDS_GREEN,
    LEDS_BLUE,
} leds_color_t;


/**
   Update LED state

 */
void leds_update(void);

/**
   Change state of the heartbeat LED

 */
void leds_heartbeat(void);

/**
   Toggle the specified LED - on/off

   \param[in] leds_color_t led color

   \return status
 */
void leds_on_off(leds_color_t const led_color);

/**
   Toggle the specified LED - PWM version

   \param[in] leds_color_t led color

   \return status
 */
void leds_toggle(leds_color_t const led_color);

/**
   Change the intestiy of the specified led in the specified direction.

   \param[in] leds_color_t led_color
   \param[in] uint8_t direction - increase or decrease

   \return status
 */
void leds_change_intensity(leds_color_t const led_color, uint8_t const direction);



#endif /* ESE_APP_LEDS_H_ */
