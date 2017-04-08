
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include <stdint.h>

#include "math-util.h"

uint32_t next_power_of_2(uint32_t num)
{
    /**
       algorithm from: http://bits.stephan-brumme.com/roundUpToNextPowerOfTwo.html

       round an integer to the next largest power of two. If the number is
       already a power of two, then return the number.

       Example, 0000_0000_0010_1101. The highest set bit is position n. set all
       bits below n, 0000_0000_0011_1111, then increment by 1,
       0000_0000_0100_0000.
    */

    if (num > 0) {
        // subtract 1 to avoid incorrect result if num is already a power of 2.
        num--;
    }
    // shift to the right and or with the number to ensure all bits below the
    // highest are are set
    num |= num >> 1;  // handle  2 bit numbers
    num |= num >> 2;  // handle  4 bit numbers
    num |= num >> 4;  // handle  8 bit numbers
    num |= num >> 8;  // handle 16 bit numbers
    num |= num >> 16; // handle 32 bit numbers
    // num has been set to to 2^(n+1) - 1, increment
    num++;

    return num;
}

