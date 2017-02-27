#ifndef ESE_UTIL_BIT_OPTS_H_
#define ESE_UTIL_BIT_OPTS_H_

#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>

/**

 */
#define MAX_SHIFT(X) (sizeof((X)) * CHAR_BIT - 1)

inline void set_bit_one(volatile uint32_t *current, uint8_t shift)
{
    assert(shift < MAX_SHIFT(*current));
    *current |= (1 << shift);
}

inline void set_bit_zero(volatile uint32_t *current, uint8_t shift)
{
    assert(shift < MAX_SHIFT(*current));
    *current &= ~(1 << shift);
}

inline void flip_bit(volatile uint32_t *current, uint8_t shift)
{
    assert(shift < MAX_SHIFT(*current));
    *current ^= (1 << shift);
}

inline void get_bit(volatile uint32_t *current, uint8_t shift, bool *value)
{
    assert(shift < MAX_SHIFT(*current));
    *value = *current & (1 << shift);
}


#endif // ESE_UTIL_BIT_OPTS_H_
