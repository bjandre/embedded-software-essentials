
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include <stdint.h>

#include "gpio.h"

#include "leds.h"

/**
   toggle the specified led
 */
void _toggle_led(leds_color_t index);

/**
   change the intensity of the specified led in the specified direction.
 */
void _change_intensity_led(leds_color_t const index, uint8_t const intensity);

void leds_update(void)
{
    gpio_toggle_red_led();
}

void leds_heartbeat(void)
{
    gpio_toggle_heartbeat_led();
}

//  FIXME(bja, 2017-08) move to HAL
static const uint8_t red_pwm_index = 0;
static const uint8_t green_pwm_index = 1;

void leds_on_off(leds_color_t const led_color)
{
    // NOTE(bja, 2017-08) invalid colors are ignored!
    if (LEDS_RED == led_color) {
        gpio_toggle_red_led();
    } else if (LEDS_GREEN == led_color) {
        gpio_toggle_green_led();
    }
    // NOTE(bja, 2017-04) blue is not included here because it is used for the
    // heartbeat
}

void leds_toggle(leds_color_t const led_color)
{
    // NOTE(bja, 2017-08) invalid colors are ignored!
    if (LEDS_RED == led_color) {
        _toggle_led(red_pwm_index);
    } else if (LEDS_GREEN == led_color) {
        _toggle_led(green_pwm_index);
    }
    // NOTE(bja, 2017-04) blue is not included here because it is used for the
    // heartbeat
}

void _toggle_led(leds_color_t const index)
{
    gpio_pwm_toggle_led(index);
}

void leds_change_intensity(leds_color_t const led_color,
                           uint8_t const direction)
{
    // NOTE(bja, 2017-08) invalid colors and changes are ignored!
    if (LEDS_RED == led_color) {
        _change_intensity_led(red_pwm_index, direction);
    } else if (LEDS_GREEN == led_color) {
        _change_intensity_led(green_pwm_index, direction);
    }

}

void _change_intensity_led(leds_color_t const index, uint8_t const direction)
{
    if (increase_intensity == direction) {
        gpio_pwm_increase_intensity(index);
    } else if (decrease_intensity == direction) {
        gpio_pwm_decrease_intensity(index);
    }
}


