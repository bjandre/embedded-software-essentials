
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifdef BARE_METAL
#else
#define TESTING_MOCK_INTERRUPT
#endif

#include <stdint.h>

#include "util.h"
#include "circular_buffer.h"
#include "log-item.h"
#include "logger.h"

#include "command.h"
#include "message.h"

#include "async-global.h"

/**
   \file message.c

   Implement a simple message format for sending and receiving messages over uart or spi.

   The basic message format is losely based on the SimpleMessage format from
   robot operating system.

   A message is: header data checksum null

   Where:
     header:
       start byte -
       length - total length of message, including start byte and checksum
       ~length - negated length, to ensure length was not corrupted

     data: arbitrary data packet

     checksum - a 1 byte checksum of the data portion of the message

     null - a null, ignored byte between messages.

*/

/**
   function pointer for massage processing state machine vtable.

   \param[in,out] message - message state struct
   \param[in] byte - byte received with this state
 */
typedef void (*message_function)(log_item_t *item, message_t *message,
                                 uint8_t byte);


/**
   function pointer for the idle state when not receiving a message

   matches call signature of message_function.
 */
void state_idle(log_item_t *item, message_t *message, uint8_t byte);

/**
   function pointer indicating that the start byte has been received

   matches call signature of message_function.
 */
void state_start(log_item_t *item, message_t *message, uint8_t byte);

/**
   function pointer indicating that the length bytes have been received

   matches call signature of message_function.
 */
void state_length_received(log_item_t *item, message_t *message, uint8_t byte);

/**
   function pointer indicating that the state machine is receiving data

   matches call signature of message_function.
 */
void state_receiving_data(log_item_t *item, message_t *message, uint8_t byte);

/**
   function pointer indicating that the state machine is receiving the checksum

   matches call signature of message_function.
 */
void state_receiving_checksum(log_item_t *item, message_t *message,
                              uint8_t byte);

/**
   function pointer indicating that the state machine is processing the message

   matches call signature of message_function.
 */
void state_message_complete(log_item_t *item, message_t *message, uint8_t byte);

/**
   vtable for message states
 */
static const message_function message_vtable[] = {
    &state_idle,
    &state_start,
    &state_length_received,
    &state_receiving_data,
    &state_receiving_checksum,
    &state_message_complete,
};

/**
   \param[in] massage
 */
void verify_checksum(log_item_t *item, message_t *message);

/**
   verify the check sum by xor ing the bytes of the data

   FIXME(bja, 2017-04) move to math utils and unit test!

   \param[in] length of data
   \param[in] pointer to data buffer
   \param[in] received checksum
   \return whether the checksum and data agree.
 */
bool verify_checksum_xor(uint8_t length, uint8_t *data, uint8_t checksum);

uint32_t process_message(log_item_t *item, message_t *message)
{
    // extract bytes from the global message buffer and process them with the
    // message state machine.
    extern volatile async_data_t global_async_data;
    bool msg_buffer_empty = false;
    do {
#ifdef TESTING_MOCK_INTERRUPT
        logger_polling_receive(1);
#endif
        CircularBufferIsEmpty(global_async_data.logger->receive_buffer,
                              &msg_buffer_empty);
        if (!msg_buffer_empty) {
            uint8_t byte;
            CircularBufferRemoveItem(global_async_data.logger->receive_buffer, &byte);
            message_vtable[message->state](item, message, byte);
        }
    } while ((!msg_buffer_empty));
    return 0u;
}

void state_idle(log_item_t *item, message_t *message, uint8_t byte)
{
    if (MESSAGE_STATE_IDLE == message->state && message_start_byte == byte) {
        message->state = MESSAGE_STATE_START;
    } else {
        // Else we are receiving data out of order. We are aren't doing
        // synchronized communication with sender, so we just log the error and
        // drop the message.
        UpdateLogItemNoPayload(item, MESSAGE_DROPPED_ERROR);
        log_item(item);
    }
}

void state_start(log_item_t *item, message_t *message, uint8_t byte)
{
    // In the start recived state, the byte we are receiving is the length.

    message->length = byte;
    message->data_length = message->length - message_header_num_bytes -
                           message_checksum_num_bytes;
    message->state = MESSAGE_STATE_LENGTH_RECEIVED;

    uint8_t max_length = message_header_num_bytes + message_data_max_bytes +
                         message_checksum_num_bytes;
    if (message->length > max_length) {
        // FIXME(bja, 2017-04) need error checking on the length!
    }
}

void state_length_received(log_item_t *item, message_t *message, uint8_t byte)
{
    // In the length received state, the byte we are receiving is the negated
    // length. From here we transition to receiving data.

    if (0x00 != (message->length & byte)) {
        UpdateLogItem(item, MESSAGE_LENGTH_ERROR, 1, &(message->length));
        log_item(item);
        UpdateLogItem(item, MESSAGE_LENGTH, 1, &byte);
        log_item(item);
    }
    message->state = MESSAGE_STATE_RECEIVING_DATA;
}

void state_receiving_data(log_item_t *item, message_t *message, uint8_t byte)
{
    // In the receiving data state, the byte we are receiving is data.  From
    // here we keep receiving data or transition receiving the checksum.
    message->data[message->data_bytes_received] = byte;
    message->data_bytes_received++;
    if (message->data_length == message->data_bytes_received) {
        message->state = MESSAGE_STATE_RECEIVING_CHECKSUM;
    }
}

void state_receiving_checksum(log_item_t *item, message_t *message,
                              uint8_t byte)
{
    // In the receiving checksum state, the byte we are receiving is a checksum.

    message->checksum[message->checksum_bytes_received] = byte;
    message->checksum_bytes_received++;
    if (message_checksum_num_bytes == message->checksum_bytes_received) {
        message->state = MESSAGE_STATE_MESSAGE_COMPLETE;
    }
}

void state_message_complete(log_item_t *item, message_t *message, uint8_t byte)
{
    // In the message complete state, we verify the checksum, put the command
    // into the command buffer, then reset the message back to the idle state.
    UNUSED_VARIABLE(byte);
    verify_checksum(item, message);
    UpdateLogItem(item, MESSAGE_COMMAND, message->data_length, message->data);
    log_item(item);
    receive_command_message(message->data_length, message->data);
    reset_message(message);
}

void verify_checksum(log_item_t *item, message_t *message)
{
    bool is_valid;
    is_valid = verify_checksum_xor(message->data_length, message->data,
                                   *(message->checksum));
    if (!is_valid) {
        UpdateLogItem(item, MESSAGE_CHECKSUM_ERROR, message_checksum_num_bytes,
                      message->checksum);
        log_item(item);
        UpdateLogItem(item, MESSAGE_CHECKSUM, message->data_length, message->data);
        log_item(item);
    }
}

bool verify_checksum_xor(uint8_t length, uint8_t *data, uint8_t checksum)
{
    for (int i = 0; i < length; i++) {
        checksum ^= *(data + i);
    }
    bool is_valid = (0x00 == checksum);
    return is_valid;
}

void reset_message(message_t *message)
{
    message->state = MESSAGE_STATE_IDLE;
    message->length = 0;
    message->data_length = 0;
    message->data_bytes_received = 0;
    message->checksum_bytes_received = 0;
}

