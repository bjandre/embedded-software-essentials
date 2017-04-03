
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef ESE_HAL_COMMUNICATION_PERIPHERAL_H_
#define ESE_HAL_COMMUNICATION_PERIPHERAL_H_

#include <stddef.h>
#include <stdint.h>

/**
   Status of communication peripheral operation
 */
typedef enum _CommStatus {
    Comm_Status_Success,
    Comm_Status_Error,
} CommStatus;

typedef struct communication_peripheral_t communication_peripheral_t;

/**
   Class defining a communication peripheral with a specified functionality and
   platform specific implementations.
 */
struct communication_peripheral_t {
    /**
       Pointer to the function containing initialization for the communication
       peripheral.

       \param[in] baud - required baud

       \return status of the operation
    */
    CommStatus (*initialize)(const uint32_t baud);

    /**
       Pointer to the function for transmitting a byte with the current
       communication peripheral.

       \param[in] byte - byte to be sent

       \return status of the operation
    */
    CommStatus (*transmit_byte)(const uint8_t byte);

    /**
       Pointer to the function for transmitting N bytes from the specified
       communication peripheral.

       \param[in] *comm - pointer to the communication peripheral
       \param[in] num_bytes - number of bytes to transmit
       \param[in] *byte - pointer to location to of bytes to transmit

       \return status of the operation
    */
    CommStatus (*transmit_n_bytes)(communication_peripheral_t *comm,
                                   const size_t num_bytes,
                                   uint8_t *bytes);

    /**
       Pointer to the function to begin an asynchronous tranmit on the
       communication peripheral

    */
    void (*begin_async_transmit)(void);

    /**
       Pointer to the function for receiving a byte from the specified
       communication peripheral.

       \param[out] *byte - pointer to location to store the received byte

       \return status of the operation
    */
    CommStatus (*receive_byte)(uint8_t *byte);

    /**
       Pointer to the function for receiving N bytes from the specified
       communication peripheral.

       \param[in] *comm - pointer to the communication peripheral
       \param[in] num_bytes - number of bytes to receive
       \param[out] *byte - pointer to location to store the received bytes

       \return status of the operation
    */
    CommStatus (*receive_n_bytes)(communication_peripheral_t *comm,
                                  const size_t num_bytes,
                                  uint8_t *bytes);

    /**
       Flush the communication peripheral transmit buffer.

       \param[in] *comm - pointer to the communication peripheral

       \return status of the operation
    */
    CommStatus (*flush_transmit_buffer)(communication_peripheral_t *comm);
};


#endif // ESE_HAL_COMMUNICATION_PERIPHERAL_H_
