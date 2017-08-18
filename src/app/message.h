/**
   \file message.h

   Routines for working with messages received over uart or spi.
 */

/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef ESE_APP_MESSAGE_H_
#define ESE_APP_MESSAGE_H_

#include <stdint.h>

#include "log_item.h"

/**
   enumeration of possible message states
 */
typedef enum message_state_t_ {
    MESSAGE_STATE_IDLE, /*!<  */
    MESSAGE_STATE_START, /*!<  */
    MESSAGE_STATE_LENGTH_RECEIVED, /*!<  */
    MESSAGE_STATE_RECEIVING_DATA, /*!<  */
    MESSAGE_STATE_RECEIVING_CHECKSUM, /*!<  */
    MESSAGE_STATE_MESSAGE_COMPLETE, /*!<  */
} message_state_t;


static const uint8_t message_header_num_bytes = 3;
#define MESSAGE_DATA_MAX_BYTES (64u)
static const uint8_t message_data_max_bytes = MESSAGE_DATA_MAX_BYTES;
#define MESSAGE_CHECKSUM_NUM_BYTES (1u)
static const uint8_t message_checksum_num_bytes = MESSAGE_CHECKSUM_NUM_BYTES;

static const uint8_t message_start_byte = 0xFE;

/**
 */
typedef struct {
    message_state_t state;
    uint8_t length;
    uint8_t data_length;
    uint8_t data_bytes_received;
    uint8_t data[MESSAGE_DATA_MAX_BYTES];
    uint8_t checksum_bytes_received;
    uint8_t checksum[MESSAGE_CHECKSUM_NUM_BYTES];
} message_t;
#undef MESSAGE_DATA_MAX_BYTES
#undef MESSAGE_CHECKSUM_NUM_BYTES

/**
   Process bytes of a message out of the global uart/spi receive buffer.

   \param[in,out] pointer to context

   \return status
 */
uint32_t process_message(log_item_t *item, message_t *message);

/**
   Clear the data in a message and assign lengths back to zero.

   \param[in,out] pointer to message

   \return status
 */
void reset_message(message_t *message);


#endif /* ESE_APP_MESSAGE_H_ */
