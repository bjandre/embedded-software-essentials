
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef ESE_APP_HEARTBEAT_H_
#define ESE_APP_HEARTBEAT_H_

#include "log-item.h"

/**
   \file heartbeat.h

   Heartbeat implementation

 */

/**
   Implement system heartbeat

   \param item log item to use for logging
   \param heartbeat_timestamp timestamp to be logged.
 */
void heartbeat(log_item_t *item);

#endif/* ESE_APP_HEARTBEAT_H_ */
