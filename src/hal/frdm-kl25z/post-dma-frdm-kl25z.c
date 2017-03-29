
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
#include "post-dma-frdm-kl25z.h"

#include "dma-frdm-kl25z.h"

POSTstatus post_dma_memset_1byte(void)
{
    size_t const dest_size = 32;
    uint8_t dma_dest[32];
    const uint8_t initial_condition = 0x55;
    const uint8_t expected = 0xAA;
    for (size_t i = 0; i < dest_size; i++) {
        *(dma_dest + i) = initial_condition;
    }
    memset_dma((uint8_t *)dma_dest, (uint8_t *)&expected, dest_size,
               sizeof(expected));
    // have to poll to verify complete.
    bool dma_complete = false;
    while (!dma_complete) {
        dma_complete = get_global_async_dma_complete();
    }
    uint8_t num_not_equal = 0;
    for (size_t i = 0; i < dest_size; i++) {
        num_not_equal += (expected != *(dma_dest + i));
    }
    POSTstatus status = POST_PASS;
    if (num_not_equal > 0) {
        status = POST_FAIL;
    }
    return status;
}

POSTstatus post_dma_memset_4byte(void)
{
    size_t const dest_size = 32;
    uint32_t dma_dest[32];
    const uint32_t initial_condition = 0xAAAAAAAA;
    const uint32_t expected = 0xdeadc0de;
    for (size_t i = 0; i < dest_size; i++) {
        *(dma_dest + i) = initial_condition;
    }
    memset_dma((uint8_t *)dma_dest, (uint8_t *)&expected, dest_size,
               sizeof(expected));
    // have to poll to verify complete.
    bool dma_complete = false;
    while (!dma_complete) {
        dma_complete = get_global_async_dma_complete();
    }
    uint8_t num_not_equal = 0;
    for (size_t i = 0; i < dest_size; i++) {
        num_not_equal += (expected != *(dma_dest + i));
    }
    POSTstatus status = POST_PASS;
    if (num_not_equal > 0) {
        status = POST_FAIL;
    }
    return status;
}

POSTstatus post_dma_memmove_1byte(void)
{
    size_t const dest_size = 32;
    uint8_t dma_src[32];
    uint8_t dma_dest[32];
    const uint8_t initial_condition = 0x55;
    for (size_t i = 0; i < dest_size; i++) {
        *(dma_src + i) = initial_condition;
        *(dma_dest + i) = 0x0;
    }
    memmove_dma((uint8_t *)dma_dest, (uint8_t *)dma_src, dest_size,
                sizeof(initial_condition));
    // have to poll to verify complete.
    bool dma_complete = false;
    while (!dma_complete) {
        dma_complete = get_global_async_dma_complete();
    }
    uint8_t num_not_equal = 0;
    for (size_t i = 0; i < dest_size; i++) {
        num_not_equal += (*(dma_src + i) != *(dma_dest + i));
    }
    POSTstatus status = POST_PASS;
    if (num_not_equal > 0) {
        status = POST_FAIL;
    }
    return status;
}

POSTstatus post_dma_memmove_4byte(void)
{
    size_t const dest_size = 32;
    uint32_t dma_src[32];
    uint32_t dma_dest[32];
    initialize_set_1((uint8_t *)dma_src, sizeof(uint32_t)*dest_size);
    for (size_t i = 0; i < dest_size; i++) {
        *(dma_dest + i) = 0x0;
    }
    memmove_dma((uint8_t *)dma_dest, (uint8_t *)dma_src, dest_size,
                sizeof(uint32_t));
    // have to poll to verify complete.
    bool dma_complete = false;
    while (!dma_complete) {
        dma_complete = get_global_async_dma_complete();
    }
    uint8_t num_not_equal = 0;
    for (size_t i = 0; i < dest_size; i++) {
        num_not_equal += (*(dma_src + i) != *(dma_dest + i));
    }
    POSTstatus status = POST_PASS;
    if (num_not_equal > 0) {
        status = POST_FAIL;
    }
    return status;
}

