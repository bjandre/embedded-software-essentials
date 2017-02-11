#include <stdint.h>
#include <stdlib.h>

#include "memory.h"

void swap(uint8_t *a, uint8_t *b);

int8_t my_memmove(uint8_t *source, uint8_t *destination, uint32_t length)
{
    if (NULL == source || NULL == destination) {
        return EXIT_FAILURE;
    }
    // Check for overlapped regions. Note that there are five possible overlap patterns:
    //
    //   * src < src + len < dest < dest + len - no overlap, start at beginning of src.
    //
    //   * dest < dest + len < src < src + len - no overlap, start at beginning of src
    //
    //   * src < dest < src + len < dest + len - overlap - start at end of src
    //
    //   * dest < src < dest + len < src + len - overlap - start at beginning of src
    //
    //   * src == dest - complete overlap, nothing to do.
    //
    // Not that of the five possible combinations, there are three operations.
    //   * Start beginning of src
    //
    //   * start at end of src
    //
    //   * do nothing

    if (source == destination) {
        // do nothing
    } else if (source < destination && destination < source + length) {
        // copy from end of source.
        uint8_t *source_pt = source + length - 1;
        uint8_t *destination_pt = destination + length - 1;
        for (int i = 0; i < length; i++) {
            *destination_pt = *source_pt;
            source_pt--;
            destination_pt--;
        }
    } else {
        // copy from begining of source.
        for (uint32_t i = 0; i < length; i++) {
            *(destination + i) = *(source + i);
        }
    }

    return EXIT_SUCCESS;
}

int8_t my_memset(uint8_t *source, uint32_t length, uint8_t value)
{
    if (NULL == source) {
        return EXIT_FAILURE;
    }

    for (uint32_t i = 0; i < length; i++) {
        *(source + i) = value;
    }
    return EXIT_SUCCESS;
}

int8_t my_memzero(uint8_t *source, uint32_t length)
{
    if (NULL == source) {
        return EXIT_FAILURE;
    }

    my_memset(source, length, 0);
    return EXIT_SUCCESS;
}

int8_t my_reverse(uint8_t *source, uint32_t length)
{
    if (NULL == source) {
        return EXIT_FAILURE;
    }

    uint8_t *begin = source;
    uint8_t *end = source + length - 1;
    while (begin < end) {
        swap(begin, end);
        begin++;
        end--;
    }
    return EXIT_SUCCESS;
}

void swap(uint8_t *a, uint8_t *b)
{
    if (NULL == a || NULL == b) {
        return;
    }

    uint8_t tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}


