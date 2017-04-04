
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
#include <stdint.h>
#include <time.h>

#include "platform-defs.h"

#if (PLATFORM == PLATFORM_FRDM)
#include "MKL25Z4.h"
#include "cmsis_gcc.h"
#define PRIMASK_IE 0U
#define PRIMASK_ID 1U
/**
   save the original state of the interrupt status from the PRIMASK register,
   then disable interrupts. This allows enabling and disabling of interrupts
   safely within nested critical regions.

   \return original state of the primask.
 */
__attribute__( ( always_inline ) ) __STATIC_INLINE uint32_t
start_critical_region(void)
{
    uint32_t original_state = __get_PRIMASK();
    __disable_irq();
    return original_state;
}

/**
   restore the global interrupt status from a saved version of the PRIMASK
   register. This allows enabling and disabling of interrupts safely within
   nested critical regions.

   \param original_state original state of the primask.
*/
__attribute__( ( always_inline ) ) __STATIC_INLINE void end_critical_region(
    uint32_t original_state)
{
    if (PRIMASK_IE == original_state) {
        __enable_irq();
    }
}
#undef PRIMASK_IE
#undef PRIMASK_ID
#else
/* platform without interrupts or threads */
#define start_critical_region() 0U;
#define end_critical_region(state) (void)state;
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
    BinaryLogger_t *logger; /*!< global logger */
    bool heartbeat_available;
    time_t heartbeat_timestamp; /*!< heartbeat timestamp */

} async_data_t;


/**
   Initialize the global data state

 */
void initialize_global_async_data(void);

/**
   Thread / interrupt safe read dma_complete from the global_async_data struct

   \return dma_complete
 */
static inline bool get_global_async_dma_complete(void)
{
    extern volatile async_data_t global_async_data;
    uint32_t interrupt_status = start_critical_region();
    bool dma_complete = global_async_data.dma_complete;
    end_critical_region(interrupt_status);
    return dma_complete;
}

/**
   Thread / interrupt safe write dma_complete from the global_async_data struct

 */
static inline void set_global_async_dma_complete(const bool dma_complete)
{
    extern volatile async_data_t global_async_data;
    uint32_t interrupt_status = start_critical_region();
    global_async_data.dma_complete = dma_complete;
    end_critical_region(interrupt_status);
}

/**
   Thread / interrupt safe read data_available from the global_async_data struct

   \return data_available
 */
static inline bool get_global_async_logger_data_available(void)
{
    extern volatile async_data_t global_async_data;
    uint32_t interrupt_status = start_critical_region();
    bool data_available = global_async_data.logger_data_available;
    end_critical_region(interrupt_status);
    return data_available;
}

/**
   Thread / interrupt safe write data_available from the global_async_data struct

 */
static inline void set_global_async_logger_data_available(
    const bool data_available)
{
    extern volatile async_data_t global_async_data;
    uint32_t interrupt_status = start_critical_region();
    global_async_data.logger_data_available = data_available;
    end_critical_region(interrupt_status);
}

/**
   Thread / interrupt safe write of logger from the global_async_data struct

 */
static inline void set_global_async_logger(BinaryLogger_t *logger)
{
    extern volatile async_data_t global_async_data;
    uint32_t interrupt_status = start_critical_region();
    global_async_data.logger = logger;
    end_critical_region(interrupt_status);
}

/**
   Thread / interrupt safe set heartbeat_available in the global_async_data struct
 */
static inline void set_global_async_heartbeat_available(
    bool heartbeat_available)
{
    extern volatile async_data_t global_async_data;
    uint32_t interrupt_status = start_critical_region();
    global_async_data.heartbeat_available = heartbeat_available;
    end_critical_region(interrupt_status);
}

/**
   Thread / interrupt safe get the heartbeat_available flag from the global_async_data struct
 */
static inline bool get_global_async_heartbeat_available(void)
{
    extern volatile async_data_t global_async_data;
    uint32_t interrupt_status = start_critical_region();
    bool heartbeat_available = global_async_data.heartbeat_available;
    end_critical_region(interrupt_status);
    return heartbeat_available;
}

/**
   Thread / interrupt safe set heartbeat_timestamp in the global_async_data struct
 */
static inline void set_global_async_heartbeat_timestamp(
    time_t heartbeat_timestamp)
{
    extern volatile async_data_t global_async_data;
    uint32_t interrupt_status = start_critical_region();
    global_async_data.heartbeat_timestamp = heartbeat_timestamp;
    end_critical_region(interrupt_status);
}

/**
   Thread / interrupt safe get the heartbeat_available from the global_async_data struct
 */
static inline time_t get_global_async_heartbeat_timestamp(void)
{
    extern volatile async_data_t global_async_data;
    uint32_t interrupt_status = start_critical_region();
    time_t heartbeat_timestamp = global_async_data.heartbeat_timestamp;
    end_critical_region(interrupt_status);
    return heartbeat_timestamp;
}

#endif // ESE_MAIN_ASYNC_GLOBAL_H_
