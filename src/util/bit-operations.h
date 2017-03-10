#ifndef ESE_UTIL_BIT_OPTS_H_
#define ESE_UTIL_BIT_OPTS_H_

#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>

/**

   MAX_SHIFT()

   Determine the maximum valid bit shift for a particular variable.

   Param: X a any valid type.

 */
#define MAX_SHIFT(X) (sizeof((X)) * CHAR_BIT - 1)

/**

   set_bit_one()

   Set the bit in the specified position to one.

   Parameters: current - pointer to the bitfield to be modified
               shift - position of the bit to modify

 */
inline void set_bit_one(volatile uint32_t *current, uint8_t shift)
{
    assert(shift <= MAX_SHIFT(*current));
    *current |= (1 << shift);
}

/**

   set_bit_zero()

   Set the bit in the specified position to zero.

   Parameters: current - pointer to the bitfield to be modified
               shift - position of the bit to modify

 */
inline void set_bit_zero(volatile uint32_t *current, uint8_t shift)
{
    assert(shift <= MAX_SHIFT(*current));
    *current &= ~(1 << shift);
}

/**

   flip_bit()

   Flip the bit in the specified position.

   Parameters: current - pointer to the bitfield to be modified
               shift - position of the bit to modify

 */
inline void flip_bit(volatile uint32_t *current, uint8_t shift)
{
    assert(shift <= MAX_SHIFT(*current));
    *current ^= (1 << shift);
}

/**

   get_bit()

   Return the boolean value of the bit in the specified position.

   Parameters: current - pointer to the bitfield to be modified
               shift - position of the bit to modify
               value - pointer to the memory for the return value

 */
inline void get_bit(volatile uint32_t *current, uint8_t shift, bool *value)
{
    assert(shift <= MAX_SHIFT(*current));
    *value = *current & (1 << shift);
}

#undef MAX_SHIFT

#endif // ESE_UTIL_BIT_OPTS_H_
