
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include <stddef.h>

#include "async-global.h"

extern bool get_global_async_dma_complete(void);
extern void set_global_async_dma_complete(const bool dma_complete);
extern bool get_global_async_logger_data_available(void);
extern void set_global_async_logger_data_available(const bool data_available);


void initialize_global_async_data(void)
{
    extern volatile async_data_t global_async_data;
    uint32_t original_state = start_critical_region();
    set_global_async_logger_data_available(false);
    set_global_async_dma_complete(false);
    set_global_async_logger(NULL);
    set_global_async_software_reset(false);
    end_critical_region(original_state);
}

