
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include "big_little.h"
#include <stdint.h>
#include <stdio.h>

void big_little(void)
{
    uint16_t num = 1;
    uint8_t *check = (uint8_t *)(&num);
    if (*check == 0) {
        printf("little\n");
    } else {
        printf("big\n");
    }
}
