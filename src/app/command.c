
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
#include <time.h>

#include "util.h"
#include "circular_buffer.h"
#include "log_item.h"
#include "command.h"
#include "leds.h"
#include "async_global.h"

/**

 */
void command_help(log_item_t *item, command_message_t const *message);
void command_echo(log_item_t *item, command_message_t const *message);
void command_get_rtc_epoch(log_item_t *item, command_message_t const *message);
void command_software_reset(log_item_t *item, command_message_t const *message);
void command_rgb_toggle(log_item_t *item, command_message_t const *message);
void command_rgb_brightness(log_item_t *item, command_message_t const *message);

typedef struct {
    command_function execute;
    char const *help;
} command_t;

static const command_t known_commands[] = {
    { &command_help, "help - list of known commands." },
    { &command_echo, "echo - echo the current command to the log." },
    { &command_get_rtc_epoch, "epoch - send epoch time." },
    { &command_software_reset, "reset - initiate a software reset." },
    { &command_rgb_toggle, "rgb toggle - toggle the specified led." },
    { &command_rgb_brightness, "rgb brightness - cycle through led brightness." },
    { NULL, ""},
};

static const size_t max_buffer_commands = 8;
static circular_buffer_t volatile *command_buffer;

command_status_t initialize_command_interpreter(void)
{
    command_status_t command_status = COMMAND_SUCCESS;
    circular_buffer_status_t cb_status = CIRCULAR_BUFFER_SUCCESS;
    const size_t bytes_per_item = sizeof(command_message_t);
    cb_status = circular_buffer_new(&command_buffer,
                                    max_buffer_commands,
                                    bytes_per_item);

    if (CIRCULAR_BUFFER_SUCCESS != cb_status) {
        abort();
    }
    return command_status;
}

void shutdown_command_interpreter(void)
{
    circular_buffer_destroy(&command_buffer);
}

void receive_command_message(uint8_t length, uint8_t *data)
{
    /**
       Unpack the command from the message buffer into a command
       structure. Store commands in a buffer until they can be processed.

       When the command is unpacked, the command is not included in the payload.
     */
    assert(length > 0);
    assert(NULL != data);
    command_message_t message;
    message.command = *data;
    message.payload_length = length - 1u;
    memmove(message.payload, data + 1, message.payload_length);

    circular_buffer_status_t cb_status = CIRCULAR_BUFFER_SUCCESS;
    bool cmd_buffer_is_full = false;
    cb_status = circular_buffer_is_full(command_buffer, &cmd_buffer_is_full);
    if (CIRCULAR_BUFFER_SUCCESS != cb_status) {
        abort();
    }
    if (!cmd_buffer_is_full) {
        cb_status = circular_buffer_add_item(command_buffer, &message);
        if (CIRCULAR_BUFFER_SUCCESS != cb_status) {
            abort();
        }
    }
}

command_status_t process_commands(log_item_t *item)
{
    command_status_t status = COMMAND_SUCCESS;
    circular_buffer_status_t cb_status = CIRCULAR_BUFFER_SUCCESS;
    bool cmd_buffer_is_empty = true;
    cb_status = circular_buffer_is_empty(command_buffer, &cmd_buffer_is_empty);
    if (CIRCULAR_BUFFER_SUCCESS != cb_status) {
        abort();
    }
    command_message_t message;
    if (!cmd_buffer_is_empty) {
        cb_status = circular_buffer_remove_item(command_buffer, &message);
        if (CIRCULAR_BUFFER_SUCCESS != cb_status) {
            abort();
        }
        known_commands[message.command].execute(item, &message);
    }
    return status;
}

void command_help(log_item_t *item, command_message_t const *message)
{
    // FIXME(bja, 2017-04) verify checksum? or was that already done when
    // the message was received...?

    UNUSED_VARIABLE(message);
    uint8_t cmd = 0;
    while (known_commands[cmd].execute != NULL) {
        uint32_t length = strlen(known_commands[cmd].help);
        log_item_update(item, HELP_COMMAND, length, known_commands[cmd].help);
        log_item(item);
        cmd++;
    }
}

void command_echo(log_item_t *item, command_message_t const *message)
{
    /**
       command has no payload
     */
    log_item_update(item, MESSAGE_COMMAND, 1, (void *) & (message->command));
    log_item(item);
    log_item_update(item, MESSAGE_LENGTH, 1, (void *) & (message->payload_length));
    log_item(item);
    log_item_update(item, MESSAGE_PAYLOAD, message->payload_length,
                    (void *) & (message->payload));
    log_item(item);
}

void command_get_rtc_epoch(log_item_t *item, command_message_t const *message)
{
    /**
       command has no payload
     */
    UNUSED_VARIABLE(item);
    UNUSED_VARIABLE(message);
    time_t timestamp = get_global_async_heartbeat_timestamp();
    binary_logger_log_data(sizeof(timestamp), (uint8_t *)(&timestamp));
}

void command_software_reset(log_item_t *item, command_message_t const *message)
{
    /**
       command has no payload
     */
    UNUSED_VARIABLE(item);
    UNUSED_VARIABLE(message);
    extern volatile async_data_t global_async_data;
    set_global_async_software_reset(true);
}

void command_rgb_toggle(log_item_t *item, command_message_t const *message)
{
    /**
       command has a 1 byte payload, the color of the led to toggle.
     */

    log_item_update(item, MESSAGE_COMMAND, 1, (void *) & (message->command));
    log_item(item);
    log_item_update(item, MESSAGE_PAYLOAD, 1, (void *) & (message->payload));
    log_item(item);

    leds_toggle(*(message->payload));

}
void command_rgb_brightness(log_item_t *item, command_message_t const *message)
{
    /**
       command has a 2 byte payload:
         byte 1 - color to change.
         byte 2 - increase or decrease brightness.
     */
    log_item_update(item, MESSAGE_COMMAND, 1, (void *) & (message->command));
    log_item(item);
    log_item_update(item, MESSAGE_PAYLOAD, 2, (void *) & (message->payload));
    log_item(item);

    uint8_t led = message->payload[0];
    uint8_t change = message->payload[1];
    leds_change_intensity(led, change);
}
