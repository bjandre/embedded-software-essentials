
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef ESE_UTIL_POST_COMMON_H_
#define ESE_UTIL_POST_COMMON_H_

/**
   \file post-common.h

   Common infrastructure shared by all power on self tests.
 */

/**
   Common return codes from all POSTs
 */
typedef enum _POSTStatus {
    POST_PASS, //!< test passed
    POST_FAIL, //!< test failed
    POST_SKIPPED, //!< test skipped
    POST_ALL_SKIPPED, //!< all tests skipped
} POSTstatus;

#endif // ESE_UTIL_POST_COMMON_H_
