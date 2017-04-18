
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include <stddef.h>
#include <stdint.h>

#include "async-global.h"

#include "data-set.h"

#include "post-common.h"
#include "post-dma.h"

#include "memory-dma.h"

POSTstatus post_dma_memset(uint32_t bytes_per_transfer)
{
    size_t const dest_size = 32;
    uint32_t dma_dest[dest_size];
    uint32_t bytes_per_item = sizeof(uint32_t);
    uint32_t total_bytes = dest_size * bytes_per_item;
    const uint32_t initial_condition = 0xAAAAAAAA;
    const uint8_t expected = 0x55;
    size_t i;
    for (i = 0; i < dest_size; i++) {
        *(dma_dest + i) = initial_condition;
    }
    memset_dma((uint8_t *)dma_dest, &expected, total_bytes, bytes_per_transfer);
    /* have to poll to verify complete. */
    bool dma_complete = false;
    while (!dma_complete) {
        dma_complete = get_global_async_dma_complete();
    }
    uint8_t num_not_equal = 0;
    for (i = 0; i < dest_size * bytes_per_item; i++) {
        num_not_equal += (expected != *((uint8_t *)dma_dest + i));
    }
    POSTstatus status = POST_PASS;
    if (num_not_equal > 0) {
        status = POST_FAIL;
    }
    return status;
}

POSTstatus post_dma_memset_1byte(void)
{
    uint32_t bytes_per_transfer = 1;
    return post_dma_memset(bytes_per_transfer);
}

POSTstatus post_dma_memset_4byte(void)
{
    uint32_t bytes_per_transfer = 1;
    return post_dma_memset(bytes_per_transfer);
}

POSTstatus post_dma_memmove(uint32_t bytes_per_transfer)
{
    size_t const dest_size = 32;
    uint32_t dma_src[dest_size];
    uint32_t dma_dest[dest_size];
    uint32_t bytes_per_item = sizeof(uint32_t);
    uint32_t total_bytes = dest_size * bytes_per_item;
    size_t i;
    initialize_set_1((uint8_t *)dma_src, total_bytes);
    for (i = 0; i < dest_size; i++) {
        *(dma_dest + i) = 0x0;
    }
    memmove_dma((uint8_t *)dma_dest, (uint8_t *)dma_src, total_bytes,
                bytes_per_transfer);
    /* have to poll to verify complete. */
    bool dma_complete = false;
    while (!dma_complete) {
        dma_complete = get_global_async_dma_complete();
    }
    uint8_t num_not_equal = 0;
    for (i = 0; i < dest_size; i++) {
        num_not_equal += (*(dma_src + i) != *(dma_dest + i));
    }
    POSTstatus status = POST_PASS;
    if (num_not_equal > 0) {
        status = POST_FAIL;
    }
    return status;
}
POSTstatus post_dma_memmove_1byte(void)
{
    uint32_t bytes_per_transfer = 1;
    return post_dma_memmove(bytes_per_transfer);
}

POSTstatus post_dma_memmove_4byte(void)
{
    uint32_t bytes_per_transfer = 4;
    return post_dma_memmove(bytes_per_transfer);
}

