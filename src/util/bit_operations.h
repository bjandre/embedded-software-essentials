
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef ESE_UTIL_BIT_OPTS_H_
#define ESE_UTIL_BIT_OPTS_H_

#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>

#include "compiler_compat.h"

/**
   Determine the maximum valid bit shift for a particular variable.

   Param: X a any valid type.

 */
#define MAX_SHIFT(X) (sizeof((X)) * CHAR_BIT - 1)

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

/**
   Set the bit in the specified position to one.

   Parameters: current - pointer to the bitfield to be modified
               shift - position of the bit to modify

 */
__STATIC_INLINE void set_bit_one(volatile uint32_t *current, uint8_t shift)
{
    assert(shift <= MAX_SHIFT(*current));
    *current |= (1 << shift);
}

/**
   Set the bit in the specified position to zero.

   Parameters: current - pointer to the bitfield to be modified
               shift - position of the bit to modify

 */
__STATIC_INLINE void set_bit_zero(volatile uint32_t *current, uint8_t shift)
{
    assert(shift <= MAX_SHIFT(*current));
    *current &= ~(1 << shift);
}

/**
   Flip the bit in the specified position.

   Parameters: current - pointer to the bitfield to be modified
               shift - position of the bit to modify

 */
__STATIC_INLINE void flip_bit(volatile uint32_t *current, uint8_t shift)
{
    assert(shift <= MAX_SHIFT(*current));
    *current ^= (1 << shift);
}

/**
   Return the boolean value of the bit in the specified position.

   Parameters: current - pointer to the bitfield to be modified
               shift - position of the bit to modify
               value - pointer to the memory for the return value

 */
__STATIC_INLINE void get_bit(volatile uint32_t *current, uint8_t shift,
                             bool *value)
{
    assert(shift <= MAX_SHIFT(*current));
    *value = *current & (1 << shift);
}

#pragma GCC diagnostic pop


#undef MAX_SHIFT

#endif/* ESE_UTIL_BIT_OPTS_H_ */
