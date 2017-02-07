#include <stdint.h>
#include <stdio.h>

#include "data.h"
#include "memory.h"

int8_t convert_endian32(uint32_t *data, uint32_t length);

int8_t *my_itoa(int8_t *string, int32_t data, int32_t base)
{
    return 0;
}

int32_t my_atoi(int8_t *string)
{
    return 0;
}

int8_t convert_endian32(uint32_t *data, uint32_t length)
{
    // big endian - most significant byte is at the lowest memory location.
    // little endian - most significant byte is at the lowest memory location.

    // conversion from big to little and little to big is the same, just swap
    // the order of bytes within each integer....
    if (data == NULL) {
        return -1;
    }
    size_t num_bytes = sizeof(*data);
    for (uint32_t i = 0; i < length; i++) {
        uint8_t *object = (uint8_t *)(data + i);
        my_reverse(object, num_bytes);
    }
    return 0;
}

int8_t big_to_little32(uint32_t *data, uint32_t length)
{
    return convert_endian32(data, length);
}

int8_t little_to_big32(uint32_t *data, uint32_t length)
{
    return convert_endian32(data, length);
}

void print_memory(uint8_t *start, uint32_t length)
{
    if (start == NULL) {
        return;
    }
    printf("%p", start);
    for (uint32_t i = 0; i < length; i++) {
        printf(" %.2x", *(start + i));
    }
}
