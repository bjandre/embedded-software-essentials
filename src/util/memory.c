#include <stdint.h>
#include <stdlib.h>

#include "memory.h"

void swap(uint8_t *a, uint8_t *b);

int8_t my_memmove(uint8_t *source, uint8_t *destination, uint32_t length)
{
    if (source == NULL) {
        return -1;
    }
    return 0;
}

int8_t my_memset(uint8_t *source, uint32_t length, uint8_t value)
{
    if (source == NULL) {
        return -1;
    }
    return 0;
}

int8_t my_memzero(uint8_t *source, uint32_t length)
{
    if (source == NULL) {
        return -1;
    }
    return 0;
}

int8_t my_reverse(uint8_t *source, uint32_t length)
{
    if (source == NULL) {
        return -1;
    }

    uint8_t *begin = source;
    uint8_t *end = source + length - 1;
    while (begin < end) {
        swap(begin, end);
        begin++;
        end--;
    }
    return 0;
}

void swap(uint8_t *a, uint8_t *b)
{
    uint8_t tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}


