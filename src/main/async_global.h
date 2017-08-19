
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

#include "compiler_compat.h"

#include "platform_defs.h"

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
__STATIC_INLINE uint32_t start_critical_region(void)
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
__STATIC_INLINE void end_critical_region(uint32_t original_state)
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

#include "binary_logger.h"
#include "profiling_timer_data.h"
#include "nrf24l01.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

/**
   \file async-global.h

   Central location to declare all asynchronously accessed global variables

 */

/**
   Struct containing global data.

 */
typedef struct AsynchronousData {
    bool dma_complete; /*!< flag indicating a dma transfer has completed. */
    bool logger_data_available; /*!< flag indicating data is available to be retreived
                            from logger receive buffer */
    binary_logger_t *logger; /*!< global logger */
    bool heartbeat_occurred;
    time_t heartbeat_timestamp; /*!< heartbeat timestamp */

    bool profiling_active; /*!< global flag indicating that profiling is active for interrupts */
    uint32_t profiling_counter_overflow; /*<! global counter for profiling timer
                                           overflows */
    profiling_timer_data_t profiling_end_time; /*<! profiling end time that can
                                                 be accessed in interrupts */
    nrf24l01p_t nrf24; /*<! global instance of nrf24l01p internal
                         state (SPI and rx, tx buffers) */
    bool software_reset_requested;
} async_data_t;


/**
   Initialize the global data state

 */
void initialize_global_async_data(void);

/**
   Thread / interrupt safe read dma_complete from the global_async_data struct

   \return dma_complete
 */
__STATIC_INLINE bool get_global_async_dma_complete(void)
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
__STATIC_INLINE void set_global_async_dma_complete(const bool dma_complete)
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
__STATIC_INLINE bool get_global_async_logger_data_available(void)
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
__STATIC_INLINE void set_global_async_logger_data_available(
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
__STATIC_INLINE void set_global_async_logger(binary_logger_t *logger)
{
    extern volatile async_data_t global_async_data;
    uint32_t interrupt_status = start_critical_region();
    global_async_data.logger = logger;
    end_critical_region(interrupt_status);
}

/**
   Thread / interrupt safe set heartbeat_occurred in the global_async_data struct
 */
__STATIC_INLINE void set_global_async_heartbeat_occurred(
    bool heartbeat_occurred)
{
    extern volatile async_data_t global_async_data;
    uint32_t interrupt_status = start_critical_region();
    global_async_data.heartbeat_occurred = heartbeat_occurred;
    end_critical_region(interrupt_status);
}

/**
   Thread / interrupt safe get the heartbeat_occurred flag from the global_async_data struct
   \return bool indicationg if a heartbeat has occurred.
 */
__STATIC_INLINE bool get_global_async_heartbeat_occurred(void)
{
    extern volatile async_data_t global_async_data;
    uint32_t interrupt_status = start_critical_region();
    bool heartbeat_occurred = global_async_data.heartbeat_occurred;
    end_critical_region(interrupt_status);
    return heartbeat_occurred;
}

/**
   Thread / interrupt safe set heartbeat_timestamp in the global_async_data struct
 */
__STATIC_INLINE void set_global_async_heartbeat_timestamp(
    time_t heartbeat_timestamp)
{
    extern volatile async_data_t global_async_data;
    uint32_t interrupt_status = start_critical_region();
    global_async_data.heartbeat_timestamp = heartbeat_timestamp;
    end_critical_region(interrupt_status);
}

/**
   Thread / interrupt safe get the heartbeat_occurred from the global_async_data struct
   \return timestamp of last heartbeat
 */
__STATIC_INLINE time_t get_global_async_heartbeat_timestamp(void)
{
    extern volatile async_data_t global_async_data;
    uint32_t interrupt_status = start_critical_region();
    time_t heartbeat_timestamp = global_async_data.heartbeat_timestamp;
    end_critical_region(interrupt_status);
    return heartbeat_timestamp;
}

/**
   Thread / interrupt safe enable profiling_active in the global_async_data struct
 */
__STATIC_INLINE void enable_global_async_profiling_active(void)
{
    extern volatile async_data_t global_async_data;
    uint32_t interrupt_status = start_critical_region();
    global_async_data.profiling_active = true;
    end_critical_region(interrupt_status);
}

/**
   Thread / interrupt safe disable profiling_active in the global_async_data struct
 */
__STATIC_INLINE void disable_global_async_profiling_active(void)
{
    extern volatile async_data_t global_async_data;
    uint32_t interrupt_status = start_critical_region();
    global_async_data.profiling_active = false;
    end_critical_region(interrupt_status);
}

/**
   Thread / interrupt safe get the profiling_active from the global_async_data struct
   \return profiling timer overflow counter
 */
__STATIC_INLINE time_t get_global_async_profiling_active(void)
{
    extern volatile async_data_t global_async_data;
    uint32_t interrupt_status = start_critical_region();
    bool profiling_active = global_async_data.profiling_active;
    end_critical_region(interrupt_status);
    return profiling_active;
}

/**
   Thread / interrupt safe increment profiling_overflow in the global_async_data struct
 */
__STATIC_INLINE void increment_global_async_profiling_overflow(void)
{
    extern volatile async_data_t global_async_data;
    uint32_t interrupt_status = start_critical_region();
    global_async_data.profiling_counter_overflow++;
    end_critical_region(interrupt_status);
}

/**
   Thread / interrupt safe get the profiling_overflow from the global_async_data struct
   \return profiling timer overflow counter
 */
__STATIC_INLINE time_t get_global_async_profiling_overflow(void)
{
    extern volatile async_data_t global_async_data;
    uint32_t interrupt_status = start_critical_region();
    time_t profiling_overflow = global_async_data.profiling_counter_overflow;
    end_critical_region(interrupt_status);
    return profiling_overflow;
}

/**
   Thread / interrupt safe set profiling_end_time in the global_async_data struct
 */
__STATIC_INLINE void set_global_async_profiling_end_time(profiling_timer_data_t
        end_time)
{
    /* NOTE(bja, 2017-04) copying struct, assumes no dynamically allocated memory! */
    extern volatile async_data_t global_async_data;
    uint32_t interrupt_status = start_critical_region();
    global_async_data.profiling_end_time = end_time;
    end_critical_region(interrupt_status);
}

/**
   Thread / interrupt safe get the profiling_overflow from the global_async_data struct
   \return profiling timer overflow counter
 */
__STATIC_INLINE profiling_timer_data_t get_global_async_profiling_end_time(void)
{
    /* NOTE(bja, 2017-04) copying struct, assumes no dynamically allocated memory! */
    extern volatile async_data_t global_async_data;
    uint32_t interrupt_status = start_critical_region();
    profiling_timer_data_t profiling_end_time =
        global_async_data.profiling_end_time;
    end_critical_region(interrupt_status);
    return profiling_end_time;
}

/**
   Thread / interrupt safe write softwane software_reset_requested from the global_async_data struct

 */
__STATIC_INLINE void set_global_async_software_reset(const bool software_reset)
{
    extern volatile async_data_t global_async_data;
    uint32_t interrupt_status = start_critical_region();
    global_async_data.software_reset_requested = software_reset;
    end_critical_region(interrupt_status);
}

/**
   Thread / interrupt safe read software_reset_requested from the global_async_data struct

   \return software_reset_requested
 */
__STATIC_INLINE bool get_global_async_software_reset(void)
{
    extern volatile async_data_t global_async_data;
    uint32_t interrupt_status = start_critical_region();
    bool software_reset = global_async_data.software_reset_requested;
    end_critical_region(interrupt_status);
    return software_reset;
}

#pragma GCC diagnostic pop

#endif/* ESE_MAIN_ASYNC_GLOBAL_H_ */
