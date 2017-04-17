
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef ESE_MAIN_POST_H_
#define ESE_MAIN_POST_H_

#include "log-item.h"

/**
   \file post.h

   Run the Power On Self Test suite.
 */

/**
   Run POST test suite

   Contains compile time platform specific code. Test status logged.
 */
void power_on_self_tests(log_item_t *log_item);

#endif/* ESE_MAIN_POST_H_ */
