
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef ESE_HAL_POST_DMA_FRDM_KL25Z_H_
#define ESE_HAL_POST_DMA_FRDM_KL25Z_H_

#include "post_common.h"

/**
   \file post-dma-frdm-kl25z.h

   Power on self tests for DMA version of memory utils.
 */

/**
   POST test for dma version of memset with 1 byte transfers
 */
POSTstatus post_dma_memset_1byte(void);

/**
   POST test for dma version of memset with 4 byte transfers
 */
POSTstatus post_dma_memset_4byte(void);

/**
   POST test for dma version of memmov with 1 byte transfers
 */
POSTstatus post_dma_memmove_1byte(void);

/**
   POST test for dma version of memmov with 4 byte transfers
 */
POSTstatus post_dma_memmove_4byte(void);

#endif/* ESE_HAL_POST_DMA_FRDM_KL25Z_H_ */
