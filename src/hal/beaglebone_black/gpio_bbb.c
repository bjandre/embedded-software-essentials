/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "gpio_common.h"
#include "gpio_bbb.h"

typedef struct _gpio_pin_t {
    bool initialized;
    char *pin_directory;
    gpio_pin_direction_t direction;
} gpio_pin_t;

// NOTE(bja, 2017-08) Accessing GPIO via the filesystem with /sys/class/gpio is
// SLOW. The kernel dosen't return until the export is complete and the pin is
// active, but udev and other external processes still need to run. As a result,
// we havo to poll the files, e.g. direction, until they are writeable by our
// user!

static const size_t MAX_STR_LENGTH = 256;
static const char export_file[] = "/sys/class/gpio/export";
static const char unexport_file[] = "/sys/class/gpio/unexport";
static const char gpio_path[] = "/sys/class/gpio";
static const char direction_filename[] = "direction";
static const char value_filename[] = "value";

typedef uint8_t gpio_pin_value_t;

static const gpio_pin_value_t pin_high = 1;
static const gpio_pin_value_t pin_low = 0;

static const gpio_pin_num_t GPIO_MIN_PIN = 0;
// 4 GPIO banks x 32 GPIO pins = 128. But examples go to 141...?

#define NUM_PINS 128
static const gpio_pin_num_t GPIO_MAX_PIN = NUM_PINS;
// FIXME(bja, 2017-08) NOT thread/interrupt safe!
static gpio_pin_t gpio_pins[NUM_PINS];
#undef NUM_PINS

/**
   Allocate and concatinate together path for pin directory.

   \param[in,out] *pin - pointer to pin structure
   \param[in] pin_number - linux gpio pin number
 */
void create_gpio_pin_directory(gpio_pin_t *pin,
                               const gpio_pin_num_t pin_number);

/**
   Set the in/out direction for a pin

   \param[in,out] *pin - pointer to pin structure
   \param[in] direction - input or output
 */
void set_pin_direction(gpio_pin_t *pin, const gpio_pin_direction_t direction);

/**
   Set the value for a bit

   \param[in,out] *pin - pointer to pin structure
   \param[in] value - high or low
 */
void set_pin_value(gpio_pin_t *pin, const gpio_pin_value_t value);

gpio_status_t gpio_bbb_initialize(void)
{
    // global stack memory should be initialized to zero?
    for (size_t pin = GPIO_MIN_PIN; pin < GPIO_MAX_PIN; pin++) {
        gpio_pins[pin].initialized = false;
    }
    return GPIO_SUCCESS;
}

gpio_status_t gpio_bbb_shutdown(void)
{

    for (size_t pin = GPIO_MIN_PIN; pin < GPIO_MAX_PIN; pin++) {
        if (gpio_pins[pin].initialized) {
            gpio_bbb_shutdown_pin(pin);
        }
    }
    return GPIO_SUCCESS;
}

gpio_status_t gpio_bbb_initialize_pin(const gpio_pin_num_t pin_number,
                                      const gpio_pin_direction_t direction)
{
    assert(pin_number >= GPIO_MIN_PIN);
    assert(pin_number < GPIO_MAX_PIN);
    FILE *fh = fopen(export_file, "w");
    assert(NULL != fh);
    fprintf(fh, "%d", pin_number);
    fclose(fh);

    gpio_pin_t *pin = gpio_pins + pin_number;
    create_gpio_pin_directory(pin, pin_number);

    set_pin_direction(pin, direction);

    pin->initialized = true;
    return GPIO_SUCCESS;
}

gpio_status_t gpio_bbb_shutdown_pin(const gpio_pin_num_t pin_number)
{
    assert(pin_number >= GPIO_MIN_PIN);
    assert(pin_number < GPIO_MAX_PIN);

    gpio_pin_t *pin = gpio_pins + pin_number;
    pin->initialized = false;
    if (NULL != pin->pin_directory) {
        free(pin->pin_directory);
    }

    FILE *fh = fopen(unexport_file, "w");
    assert(NULL != fh);
    fprintf(fh, "%d", pin_number);
    fclose(fh);

    return GPIO_SUCCESS;
}

gpio_status_t gpio_bbb_set_pin(const gpio_pin_num_t pin_number)
{
    assert(pin_number >= GPIO_MIN_PIN);
    assert(pin_number < GPIO_MAX_PIN);

    gpio_pin_t *pin = gpio_pins + pin_number;
    assert(true == pin->initialized);

    set_pin_value(pin, pin_high);

    return GPIO_SUCCESS;
}

gpio_status_t gpio_bbb_clear_pin(const gpio_pin_num_t pin_number)
{
    assert(pin_number >= GPIO_MIN_PIN);
    assert(pin_number < GPIO_MAX_PIN);

    gpio_pin_t *pin = gpio_pins + pin_number;
    assert(true == pin->initialized);

    set_pin_value(pin, pin_low);

    return GPIO_SUCCESS;
}


void create_gpio_pin_directory(gpio_pin_t *pin, const gpio_pin_num_t pin_number)
{
    assert(NULL != pin);
    pin->pin_directory = calloc(MAX_STR_LENGTH, sizeof(char));
    assert(NULL != pin->pin_directory);
    sprintf(pin->pin_directory, "%s/gpio%d", gpio_path, pin_number);
}

void set_pin_direction(gpio_pin_t *pin, const gpio_pin_direction_t direction)
{
    assert(NULL != pin);
    char direction_file[MAX_STR_LENGTH];
    snprintf(direction_file, MAX_STR_LENGTH, "%s/%s", pin->pin_directory,
             direction_filename);
    while (access(direction_file, W_OK | F_OK)) {
        __asm("NOP");
    }
    FILE *fh = fopen(direction_file, "w");
    if (GPIO_INPUT == direction) {
        fprintf(fh, "in");
    } else if (GPIO_OUTPUT == direction) {
        fprintf(fh, "out");
    } else {
        assert(false);
    }
    pin->direction = direction;
    fclose(fh);
}

void set_pin_value(gpio_pin_t *pin, const uint8_t value)
{
    assert(NULL != pin);
    char value_file[MAX_STR_LENGTH];
    snprintf(value_file, MAX_STR_LENGTH, "%s/%s", pin->pin_directory,
             value_filename);
    while (access(value_file, W_OK | F_OK)) {
        __asm("NOP");
    }
    FILE *fh = fopen(value_file, "w");
    fprintf(fh, "%d", value);
    fclose(fh);
}
