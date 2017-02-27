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
