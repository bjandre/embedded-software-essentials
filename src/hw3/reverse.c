
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include <stdlib.h>

void swap(char *a, char *b);

void swap(char *a, char *b)
{
    char tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

char reverse(char *str, int length)
{
    if (length < 0 || str == NULL) {
        return -1;
    }
    char *begin = str;
    char *end = str + length - 1;
    while (begin < end) {
        swap(begin, end);
        begin++;
        end--;
    }
    return 0;
}

