
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

#include "memory-common.h"
#include "memory-cpu.h"

MemStatus memmove_cpu(uint8_t *source, uint8_t *destination, uint32_t length)
{
    if (NULL == source || NULL == destination) {
        return MemStatus_Null_Pointer;
    }
    // Check for overlapped regions. Note that there are five possible overlap patterns:
    //
    //   * src < src + len < dest < dest + len - no overlap, start at beginning of src.
    //
    //   * dest < dest + len < src < src + len - no overlap, start at beginning of src
    //
    //   * src < dest < src + len < dest + len - overlap - start at end of src
    //
    //   * dest < src < dest + len < src + len - overlap - start at beginning of src
    //
    //   * src == dest - complete overlap, nothing to do.
    //
    // Not that of the five possible combinations, there are three operations.
    //   * Start beginning of src
    //
    //   * start at end of src
    //
    //   * do nothing

    if (source == destination) {
        // do nothing
    } else if (source < destination && destination < source + length) {
        // copy from end of source.
        uint8_t *source_pt = source + length - 1;
        uint8_t *destination_pt = destination + length - 1;
        for (int i = 0; i < length; i++) {
            *destination_pt = *source_pt;
            source_pt--;
            destination_pt--;
        }
    } else {
        // copy from begining of source.
        for (uint32_t i = 0; i < length; i++) {
            *(destination + i) = *(source + i);
        }
    }

    return MemStatus_Success;
}

MemStatus memset_cpu(uint8_t *source, uint32_t length, uint8_t value)
{
    if (NULL == source) {
        return MemStatus_Null_Pointer;
    }

    for (uint32_t i = 0; i < length; i++) {
        *(source + i) = value;
    }
    return MemStatus_Success;
}

