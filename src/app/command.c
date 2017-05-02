
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "circular_buffer.h"
#include "log-item.h"
#include "command.h"
#include "async-global.h"
/**

 */
void command_help(log_item_t *item, command_message_t const *message);
void command_echo(log_item_t *item, command_message_t const *message);
void command_get_rtc_epoch(log_item_t *item, command_message_t const *message);
void command_software_reset(log_item_t *item, command_message_t const *message);

typedef struct {
    command_function execute;
    char const *help;
} command_t;

static const command_t known_commands[] = {
    { &command_help, "help - list of known commands." },
    { &command_help, "echo - echo the current command to the log." },
    { &command_get_rtc_epoch, "epoch - send epoch time." },
    { &command_software_reset, "reset - initiate a software reset." },
    { NULL, ""},
};

static const size_t max_buffer_commands = 8;
static CircularBuffer_t volatile *command_buffer;

command_status_t initialize_command_interpreter(void)
{
    command_status_t command_status = COMMAND_SUCCESS;
    CircularBufferStatus cb_status = CircularBuffer_Success;
    const size_t bytes_per_item = sizeof(command_message_t);
    cb_status = CircularBufferNew(&command_buffer,
                                  max_buffer_commands,
                                  bytes_per_item);

    if (CircularBuffer_Success != cb_status) {
        abort();
    }
    return command_status;
}

void receive_command_message(uint8_t length, uint8_t *data)
{
    assert(length > 0);
    assert(NULL != data);
    command_message_t message;
    message.command = *data;
    message.payload_length = length - 1u;
    memmove(message.payload, data + 1, message.payload_length);

    CircularBufferStatus cb_status = CircularBuffer_Success;
    bool cmd_buffer_is_full = false;
    cb_status = CircularBufferIsFull(command_buffer, &cmd_buffer_is_full);
    if (CircularBuffer_Success != cb_status) {
        abort();
    }
    if (!cmd_buffer_is_full) {
        cb_status = CircularBufferAddItem(command_buffer, &message);
        if (CircularBuffer_Success != cb_status) {
            abort();
        }
    }
}

command_status_t process_commands(log_item_t *item)
{
    command_status_t status = COMMAND_SUCCESS;
    CircularBufferStatus cb_status = CircularBuffer_Success;
    bool cmd_buffer_is_empty = true;
    cb_status = CircularBufferIsEmpty(command_buffer, &cmd_buffer_is_empty);
    if (CircularBuffer_Success != cb_status) {
        abort();
    }
    command_message_t message;
    if (!cmd_buffer_is_empty) {
        cb_status = CircularBufferRemoveItem(command_buffer, &message);
        if (CircularBuffer_Success != cb_status) {
            abort();
        }
        known_commands[message.command].execute(item, &message);
    }
    return status;
}

void command_help(log_item_t *item, command_message_t const *message)
{
    UNUSED_VARIABLE(message);
    // FIXME(bja, 2017-04) verify checksum? or was that already done when
    // the message was received...?
    uint8_t cmd = 0;
    while (known_commands[cmd].execute != NULL) {
        uint32_t length = strlen(known_commands[cmd].help);
        UpdateLogItem(item, HELP_COMMAND, length, known_commands[cmd].help);
        log_item(item);
        cmd++;
    }
}

void command_echo(log_item_t *item, command_message_t const *message)
{
    // FIXME(bja, 2017-04) verify checksum? or was that already done when
    // the message was received...?

    UpdateLogItem(item, MESSAGE_COMMAND, 1, (void *) & (message->command));
    log_item(item);
    UpdateLogItem(item, MESSAGE_LENGTH, 1, (void *) & (message->payload_length));
    log_item(item);
    uint8_t payload_length = message->payload_length - 3u;
    UpdateLogItem(item, MESSAGE_PAYLOAD, payload_length,
                  (void *) & (message->payload));
    log_item(item);
}

void command_get_rtc_epoch(log_item_t *item, command_message_t const *message)
{
    UNUSED_VARIABLE(item);
    UNUSED_VARIABLE(message);
}

void command_software_reset(log_item_t *item, command_message_t const *message)
{
    UNUSED_VARIABLE(item);
    UNUSED_VARIABLE(message);
    extern volatile async_data_t global_async_data;
    set_global_async_software_reset(true);
}
