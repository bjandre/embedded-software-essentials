
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef ESE_MAIN_ASYNC_GLOBAL_H_
#define ESE_MAIN_ASYNC_GLOBAL_H_

#include <stdbool.h>

#include "platform-defs.h"

#if (PLATFORM == PLATFORM_FRDM)
#include "MKL25Z4.h"
#define start_critical_region() __disable_irq()
#define end_critical_region() __enable_irq()
#else
#define start_critical_region()
#define end_critical_region()
#endif

#include "logger.h"

/**
   \file async-global.h

   Central location to declare all asynchronously accessed global variables

   FIXME(bja, 2017-03) Need to implement status preservation for critical region
   so nested calls work. This is needed to correctly wrap the logger calls.

 */

/**
   Struct containing global data.

 */
typedef struct AsynchronousData {
    bool dma_complete; /*!< flag indicating a dma transfer has completed. */
    bool logger_data_available; /*!< flag indicating data is available to be retreived
                            from logger receive buffer */
    BinaryLogger_t logger; /*!< global logger */

} async_data_t;


/**
   Thread / interrupt safe read dma_complete from the global_async_data struct

   \return dma_complete
 */
static inline bool get_global_async_dma_complete(void)
{
    extern volatile async_data_t global_async_data;
    start_critical_region();
    bool dma_complete = global_async_data.dma_complete;
    end_critical_region();
    return dma_complete;
}

/**
   Thread / interrupt safe write dma_complete from the global_async_data struct

 */
static inline void set_global_async_dma_complete(const bool dma_complete)
{
    extern volatile async_data_t global_async_data;
    start_critical_region();
    global_async_data.dma_complete = dma_complete;
    end_critical_region();
}

/**
   Thread / interrupt safe read data_available from the global_async_data struct

   \return data_available
 */
static inline bool get_global_async_logger_data_available(void)
{
    extern volatile async_data_t global_async_data;
    start_critical_region();
    bool data_available = global_async_data.logger_data_available;
    end_critical_region();
    return data_available;
}

/**
   Thread / interrupt safe write data_available from the global_async_data struct

 */
static inline void set_global_async_logger_data_available(
    const bool data_available)
{
    extern volatile async_data_t global_async_data;
    start_critical_region();
    global_async_data.logger_data_available = data_available;
    end_critical_region();
}

#endif // ESE_MAIN_ASYNC_GLOBAL_H_
