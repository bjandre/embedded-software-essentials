
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include <stdint.h>

#include "platform_defs.h"
#if (PLATFORM == PLATFORM_FRDM)
#include "gpio_frdm_kl25z.h"
#include "MKL25Z4.h"
#elif (PLATFORM == PLATFORM_BBB)
#include "gpio_bbb.h"
#endif

#include "gpio_common.h"
#include "gpio.h"

void gpio_set_pin(gpio_pin_num_t const pin)
{
#if (PLATFORM == PLATFORM_FRDM)
    GPIOD->PSOR |= (1 << pin);
#elif (PLATFORM == PLATFORM_BBB)
    gpio_bbb_set_pin(pin);
#endif
}

void gpio_clear_pin(gpio_pin_num_t const pin)
{
#if (PLATFORM == PLATFORM_FRDM)
    GPIOD->PCOR |= (1 << pin);
#elif (PLATFORM == PLATFORM_BBB)
    gpio_bbb_clear_pin(pin);
#endif
}

void gpio_initialize_chip_active_pin(void)
{
#if (PLATFORM == PLATFORM_FRDM)
    frdm_kl25z_initialize_port_d_output_pin(NRF24_CHIP_ACTIVATE);
#elif (PLATFORM == PLATFORM_BBB)
    gpio_bbb_initialize_pin(NRF24_CHIP_ACTIVATE, GPIO_OUTPUT);
#endif
}

void gpio_toggle_heartbeat_led(void)
{
#if (PLATFORM == PLATFORM_FRDM)
    frdm_kl25z_heartbeat_led();
#endif
}

void gpio_toggle_red_led(void)
{
#if (PLATFORM == PLATFORM_FRDM)
    frdm_kl25z_toggle_red_led();
#endif
}

void gpio_toggle_green_led(void)
{
#if (PLATFORM == PLATFORM_FRDM)
    frdm_kl25z_toggle_green_led();
#endif
}

void gpio_pwm_toggle_led(uint32_t const index)
{
#if (PLATFORM == PLATFORM_FRDM)
    if (TPM2->CONTROLS[index].CnV == 0) {
        TPM2->CONTROLS[index].CnV = TPM2->MOD / 8;
    } else {
        TPM2->CONTROLS[index].CnV = 0;
    }
#endif
}

void gpio_pwm_increase_intensity(uint32_t const index)
{
#if (PLATFORM == PLATFORM_FRDM)
    TPM2->CONTROLS[index].CnV <<= 1;
#endif
}

void gpio_pwm_decrease_intensity(uint32_t const index)
{
#if (PLATFORM == PLATFORM_FRDM)
    TPM2->CONTROLS[index].CnV >>= 1;
#endif
}
