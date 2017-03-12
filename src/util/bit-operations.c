
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "bit-operations.h"

extern void set_bit_one(volatile uint32_t *current, uint8_t bit_num);
extern void set_bit_zero(volatile uint32_t *current, uint8_t bit_num);
extern void flip_bit(volatile uint32_t *current, uint8_t bit_num);
extern void get_bit(volatile uint32_t *current, uint8_t position, bool *value);
