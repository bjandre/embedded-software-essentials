
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifndef BARE_METAL
#define TESTING_MOCK_INTERRUPT
#endif

#include "logger.h"
#include "log-item.h"
#include "analyze-data.h"

#include "async-global.h"

uint8_t const num_required = 16;

void initialize_logger_data_analysis(data_summary_t *data_summary,
                                     log_item_t *item)
{
    clear_data_summary(data_summary);

    UpdateLogItemNoPayload(item, DATA_ANALYSIS_STARTED);
    log_item(item);
}

void analyze_logger_data_event(data_summary_t *data_summary, log_item_t *item)
{
    uint8_t byte = 0x00;
    bool data_available = false;
    BinaryLoggerStatus logger_status = BinaryLogger_Success;

#ifdef TESTING_MOCK_INTERRUPT
    set_global_async_logger_data_available(true);
#endif

    data_available = get_global_async_logger_data_available();
    set_global_async_logger_data_available(false);

    if (data_available) {
        log_receive_data(1, &byte);
        logger_status = UpdateLogItem(item, DATA_RECEIVED, 1, &byte);
        if (BinaryLogger_Success != logger_status) {
            abort();
        }
        log_item(item);
        data_summary->num_received++;
        process_data(data_summary, byte);
    }

    if (data_summary->num_received == num_required) {
        log_data_analysis(item, data_summary);
        UpdateLogItemNoPayload(item, DATA_ANALYSIS_COMPLETED);
        log_item(item);
        clear_data_summary(data_summary);
        data_summary->num_received = 0;
#ifdef TESTING_MOCK_INTERRUPT
        exit(EXIT_SUCCESS);
#endif
    }
}

void clear_data_summary(data_summary_t *data_summary)
{
    if (NULL != data_summary) {
        data_summary->num_received = 0;
        data_summary->num_alphabetic = 0;
        data_summary->num_numeric = 0;
        data_summary->num_punctuation = 0;
        data_summary->num_misc = 0;
    }
}

void process_data(data_summary_t *data_summary, uint8_t byte)
{
    /*
      ASCII Table, row is first hex digit, column is second hex digit

       *    0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
       0    NUL SOH STX ETX EOT ENQ ACK BEL BS  TAB LF  VT  FF  CR  SO  SI
       1    DLE DC1 DC2 DC3 DC4 NAK SYN ETB CAN EM  SUB ESC FS  GS  RS  US
       2        !   "   #   $   %   &   '   (   )   *   +   ,   -   .   /
       3    0   1   2   3   4   5   6   7   8   9   :   ;   <   =   >   ?
       4    @   A   B   C   D   E   F   G   H   I   J   K   L   M   N   O
       5    P   Q   R   S   T   U   V   W   X   Y   Z   [   \   ]   ^   _
       6    `   a   b   c   d   e   f   g   h   i   j   k   l   m   n   o
       7    p   q   r   s   t   u   v   w   x   y   z   {   |   }   ~

       so 0x41 = 'A'

       source: http://www.cplusplus.com/doc/ascii/

    */

    if (NULL != data_summary) {
        if (byte >= 0x30 && byte <= 0x39) {
            data_summary->num_numeric++;
        } else if ((byte >= 0x41 && byte <= 0x5A) ||
                   (byte >= 0x61 && byte <= 0x7A)) {
            data_summary->num_alphabetic++;
        } else if ((byte >= 0x21 && byte <= 0x2F) ||
                   (byte >= 0x3A && byte <= 0x40) ||
                   (byte >= 0x5B && byte <= 0x60) ||
                   (byte >= 0x7B && byte <= 0x7E)) {
            data_summary->num_punctuation++;
        } else {
            data_summary->num_misc++;
        }
    }
}

void log_data_analysis(log_item_t *item, data_summary_t *summary)
{
    UpdateLogItem(item, DATA_ALPHA_COUNT, sizeof(summary->num_alphabetic),
                  &(summary->num_alphabetic));
    log_item(item);

    UpdateLogItem(item, DATA_NUMERIC_COUNT, sizeof(summary->num_numeric),
                  &(summary->num_numeric));
    log_item(item);

    UpdateLogItem(item, DATA_PUNCTUATION_COUNT, sizeof(summary->num_punctuation),
                  &(summary->num_punctuation));
    log_item(item);

    UpdateLogItem(item, DATA_MISC_COUNT, sizeof(summary->num_misc),
                  &(summary->num_misc));
    log_item(item);
}
