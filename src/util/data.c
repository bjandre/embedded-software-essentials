#include <stdint.h>
#include <stdio.h>

#include "data.h"
#include "memory.h"

int8_t convert_endian32(uint32_t *data, uint32_t length);

int8_t *my_itoa(int8_t *string, int32_t data, int32_t base)
{
    if (string != NULL) {
        // offset for ascii characters
        int8_t ascii_offset = '0';
        // offset for ascii  hex characters > 9
        int8_t ascii_offset_hex = 'a' - 0x0a;
        // output hex
        uint32_t length = 0;
        int8_t *position = string;
        *position = '\0';
        position++;
        length++;
        int8_t negative = 0;
        if (data < 0) {
            negative = 1;
            data = -data;
        }
        do {
            int8_t remainder = data % base;
            if (remainder > 9) {
                *position = remainder + ascii_offset_hex;
            } else {
                *position = remainder + ascii_offset;
            }
            position++;
            length++;
            data /= base;
        } while (data > 0);

        if (negative) {
            *position = '-';
            position++;
            length++;
        }
        if (0) {
            printf("itoa length = %d   string = '%s'\n", length, string);
            for (uint32_t i = 0; i < 32; i++) {
                printf("<%c>", string[i]);
            }
            printf("\n");
        }

        my_reverse((uint8_t *)string, length);

        if (0) {
            printf("itoa string = '%s'\n", string);
            for (uint32_t i = 0; i < 32; i++) {
                printf("<%c>", string[i]);
            }
            printf("\n");
        }
    }
    return string;
}

int32_t my_atoi(int8_t *string)
{
    // NOTE(bja, 2017-02) this is extremely brittle and does not do any sanity
    // checking of user input.

    // * does not handle situations where non-digit characteres are present
    // correctly. E.g. 10,234 is treated the same as 12egd345.  * does not
    // handle sign correctly if sign is in the interior of the string.
    //
    // Should really strip the leading whitespace characters.

    // Check for a sign character.

    // parse any digit characters until the first non-digit character.

    // assumes that digits are stored in the string such that the most
    // siginificant digit is at the lowest index in the string, e.g.
    // 1002 = str = {' ', ' ', '1', '0', '0', '2', '\0'}
    if (string == NULL) {
        return 0;
    }
    int32_t value = 0;
    uint32_t sign = 1;
    int8_t *position = string;
    while (*position != '\0') {
        if (*position >= '0' && *position <= '9') {
            value = 10 * value + (*position - '0');
        } else if (*position == '-') {
            sign = -1;
        }
        position++;
    }
    return sign * value;
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
