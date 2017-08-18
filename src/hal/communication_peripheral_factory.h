
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef ESE_HAL_COMMUNICATION_PERIPHERAL_FACTORY_H_
#define ESE_HAL_COMMUNICATION_PERIPHERAL_FACTORY_H_

#include "communication_peripheral.h"

/**
   Type of communication peripheral functionality
 */
typedef enum _CommFunction {
    UART_Logger,
    UART_General,
} CommFunction;

/**
   \file communication-peripheral-factory.h

   Interface to create an instance of a communication peripheral
 */

/**
   Create a communication peripheral data strucure with pointers to the correct
   functions for the desired functionality, e.g. logger uart, spi peripheral.

   \param[out] comm pointer to the struct of function pointers.
   \param[in] comm_function the type of comm functionality

   \return status of the create operation
 */
CommStatus CommCreate(communication_peripheral_t volatile *comm,
                      CommFunction comm_function);


/**
   Destroy a communication peripheral object.

   \param[in] comm pointer to the struct of function pointers.

   \return status of the create operation
 */
void CommDestroy(communication_peripheral_t volatile *comm);

#endif/* ESE_HAL_COMMUNICATION_PERIPHERAL_FACTORY_H_ */
