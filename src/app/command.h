/**
   \file command.h

   Command interpreter
 */

/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef ESE_APP_COMMAND_H_
#define ESE_APP_COMMAND_H_

#include <stdint.h>

#include "log-item.h"

/**

 */
typedef enum {
    COMMAND_SUCCESS,
    COMMAND_ERROR,
} command_status_t;

/**
   Known commands.

   Since we are receiving commands as messages over uart/spi, we need to ensure
   they are of a fixed size.
 */
typedef uint8_t command_id_t;
static const command_id_t COMMAND_HELP = 0; /*!< help */
static const command_id_t COMMAND_ECHO = 1; /*!< echo */
static const command_id_t COMMAND_GET_RTC_EPOCH = 2; /*!<  */
static const command_id_t COMMAND_SOFTWARE_RESET = 3; /*!< */
static const command_id_t COMMAND_RGB_ON_OFF = 4; /*!< */
static const command_id_t COMMAND_RGB_BRIGHTNESS = 5; /*!< */
static const command_id_t COMMAND_RGB_COLOR = 6; /*!< */

/**
   Command message
 */
static const uint8_t num_bytes_command = sizeof(command_id_t);
#define MAX_COMMAND_PAYLOAD_BYTES (64u)
static const uint8_t max_command_payload_bytes = MAX_COMMAND_PAYLOAD_BYTES;

typedef struct {
    command_id_t command;
    size_t payload_length;
    uint8_t payload[MAX_COMMAND_PAYLOAD_BYTES];
} command_message_t;
#undef MAX_COMMAND_PAYLOAD_BYTES

typedef void (*command_function)(log_item_t *item,
                                 command_message_t const *message);

/**
   Initialize the virtual table for the command interpreter

   \param[in,out] pointer to context

   \return status
 */
command_status_t initialize_command_interpreter(void);

/**
   receive commands into the command buffer
   \param[in] length of command + data
   \param[in] pointer to buffer containing command and data
 */
void receive_command_message(uint8_t length, uint8_t *data);

/**
   process commands from the command buffer

   \return status
 */
command_status_t process_commands(log_item_t *item);

#endif /* ESE_APP_COMMAND_H_ */
