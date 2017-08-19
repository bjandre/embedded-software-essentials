
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef ESE_APP_LOGGER_H_
#define ESE_APP_LOGGER_H_

/**
   \file logger.h

   Binary Logger implemenation. Implementation is flexible and works on a
   generic communication peripheral, which can be hardware uart, spi or software
   (printf/getc).

   The logger is global and accessed in critical regions, e.g. interrupts.  All
   instances must be volatile.

 */

#include <stdint.h>

#include "circular_buffer.h"
#include "communication_peripheral.h"

/**
   Buffers and communication peripheral associated with the logger
 */
typedef struct {
    circular_buffer_t volatile *transmit_buffer;
    circular_buffer_t volatile *receive_buffer;
    communication_peripheral_t comm;
} BinaryLogger_t;

/**
   Status codes returned by binary logger operations
 */
typedef enum {
    BinaryLogger_Success,
    BinaryLogger_Error,
    BinaryLogger_Null_Pointer,
    BinaryLogger_Item_Alloc_Error,
    BinaryLogger_Null_Item,
    BinaryLogger_Null_Data,
} BinaryLoggerStatus;

/**
   define a type for for the sizes used to interact with the logger
*/
typedef uint8_t logger_size_t;

/**
   Initialize the global binary logger to the specified nuber of bytes

   \param[in] num_bytes - number of bytes in the send and receive buffers.

   \return status of the operation
 */
BinaryLoggerStatus BinaryLoggerCreate(logger_size_t num_bytes);

/**
   Destroy the global binary logger and free all internal memory

   \return status of the operation
 */
void BinaryLoggerDestroy(void);

/**
   Log the specified number of bytes from the provided buffer.

   \param[in] num_bytes - number of bytes to log
   \param[in] buffer - pointer to the data to be logged.

   \return status of the operation
 */
BinaryLoggerStatus log_data(logger_size_t num_bytes, uint8_t *buffer);

/**
   Log the null terminated c string.

   \param[in] string - pointer to the null terminated c string to be logged

   \return status of the operation
 */
BinaryLoggerStatus log_string(uint8_t *string);

/**
   Log the 32 bit integer

   \param[in] integer - integer to be logged

   \return status of the operation
 */
BinaryLoggerStatus log_integer(int32_t integer);


/**
   Poll the transmit buffer until it returns empty

   \return status of the operation
 */
BinaryLoggerStatus log_flush(void);

/**
   Extract data from the receive buffer and return it to the user.

   Polls the circular buffer until the requested number of bytes are
   available. To avoid polling, request one byte at a time.

   The global_async_data.data_available flag will be set when data is available
   in the receive buffer.

   \param[in] num_bytes - number of bytes to extract from the receive circular
                       buffer and return to the user.

   \param[out] buffer - pointer to user supplied buffer to store the data
                    in. Assumes that the buffer is large enough to hold the
                    requested number of bytes.

   \return status of the operation
 */
BinaryLoggerStatus log_receive_data(logger_size_t num_bytes, uint8_t *buffer);

/**
   logger receive using polling

   \param num_bytes number of bytes to receive
 */
BinaryLoggerStatus logger_polling_receive(logger_size_t num_bytes);

#endif/* ESE_APP_LOGGER_H_ */
