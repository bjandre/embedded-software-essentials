
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef ESE_UTIL_MATH_UTIL_H_
#define ESE_UTIL_MATH_UTIL_H_

#include <stdint.h>

/**
    Round an integer to the next largest power of two. If the number is already
    a power of two, then return the number.

    \param num current number

    \return next highest power of 2
*/
uint32_t next_power_of_2(uint32_t num);



#endif // ESE_UTIL_MATH_UTIL_H_
