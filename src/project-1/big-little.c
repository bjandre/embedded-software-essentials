#include "big-little.h"
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
