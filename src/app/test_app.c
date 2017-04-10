
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include <stdint.h>
#include <stdio.h>

#include "test_profiler.h"

/**
   main test driver for all platform independent utilities
 */
int main(int argc, char **argv)
{
    int status = 0;
    status += suite_profiler();

    return status;
}

