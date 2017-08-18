
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include <stdint.h>
#include <stdlib.h>

#include "memory_common.h"
#include "memory_cpu.h"

void swap(uint8_t *a, uint8_t *b);
MemStatus my_memzero(uint8_t *destination, uint32_t length)
{
    if (NULL == destination) {
        return MemStatus_Null_Pointer;
    }
    const uint8_t zero = 0;
    memset_cpu(destination, &zero, length);
    return MemStatus_Success;
}

MemStatus my_reverse(uint8_t *source, uint32_t length)
{
    if (NULL == source) {
        return MemStatus_Null_Pointer;
    }

    uint8_t *begin = source;
    uint8_t *end = source + length - 1;
    while (begin < end) {
        swap(begin, end);
        begin++;
        end--;
    }
    return MemStatus_Success;
}

void swap(uint8_t *a, uint8_t *b)
{
    if (NULL == a || NULL == b) {
        return;
    }

    uint8_t tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}


