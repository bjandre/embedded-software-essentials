
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include <assert.h>
#include <stdint.h>

#include "data_set.h"

void initialize_set_1(uint8_t *set_1, uint32_t size)
{
    /* setting memory to repeating blocks of 0x0123456789abcdef is eight bytes. */
    const uint32_t bytes_per_block = 8;
    assert(size % bytes_per_block == 0);
    int num_blocks = size / bytes_per_block;
    for (uint32_t block = 0; block < num_blocks ; block++) {
        uint32_t block_start = block * bytes_per_block;
        uint8_t *position = set_1 + block_start;
        /* set the first byte in the block. */
        *(position) = 0x01;
        for (uint32_t byte = 0; byte < bytes_per_block; byte++) {
            position++;
            *position = *(position - 1) + 0x22;
        }
    }

#   ifdef DEBUG_VERBOSE
    printf("set_1 :\n  ");
    print_memory(set_1, size);
    printf("\n");
#   endif
}

void initialize_set_2(uint8_t *set_2, uint32_t size)
{
    for (uint32_t byte = 0; byte < size; byte++) {
        *(set_2 + byte) = byte;
    }

#   ifdef DEBUG_VERBOSE
    printf("set_2 :\n  ");
    print_memory(set_2, size);
    printf("\n");
#   endif
}

void initialize_set_3(uint8_t *set_3, uint32_t size)
{
    /* FIXME(bja, 2017-02) unused in project 1. Need to keep track of the number */
    /* of bytes initialized. Start with a base of 'a'. If we cross 26, then need */
    /* to skip back to 'A' as the base. After we cross another 26, we reset the */
    /* base to 'a', etc. */
    for (uint32_t byte = 0; byte < size; byte++) {
        *(set_3 + byte) = 'a' + byte;
    }

#   ifdef DEBUG_VERBOSE
    printf("set_3 :\n  ");
    print_memory(set_3, size);
    printf("\n");
#   endif
}

