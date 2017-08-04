
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
#include "memory-dma.h"

#include "platform-defs.h"
#if (PLATFORM == PLATFORM_FRDM)
#include "dma-frdm-kl25z.h"
#elif (PLATFORM == PLATFORM_BBB)
#include "dma-bbb.h"
#else
#include "dma-host.h"
#endif

MemStatus memmove_dma(uint8_t *destination, const uint8_t *const source,
                      uint32_t const num_bytes, uint32_t const bytes_per_transfer)
{
    if (NULL == source || NULL == destination) {
        return MemStatus_Null_Pointer;
    }

#include "platform-defs.h"
#if (PLATFORM == PLATFORM_FRDM)
    frdm_kl25z_memmove_dma(destination, source, num_bytes, bytes_per_transfer);
#elif (PLATFORM == PLATFORM_BBB)
    bbb_memmove_dma(destination, source, num_bytes, bytes_per_transfer);
#else
    host_memmove_dma(destination, source, num_bytes, bytes_per_transfer);
#endif

    return MemStatus_Success;
}

MemStatus memset_dma(uint8_t *destination, const uint8_t *const source,
                     uint32_t const num_bytes, uint32_t const bytes_per_transfer)
{
    if (NULL == destination || NULL == source) {
        return MemStatus_Null_Pointer;
    }

#include "platform-defs.h"
#if (PLATFORM == PLATFORM_FRDM)
    frdm_kl25z_memset_dma(destination, source, num_bytes, bytes_per_transfer);
#elif (PLATFORM == PLATFORM_BBB)
    bbb_memset_dma(destination, source, num_bytes, bytes_per_transfer);
#else
    host_memset_dma(destination, source, num_bytes, bytes_per_transfer);
#endif

    return MemStatus_Success;
}

