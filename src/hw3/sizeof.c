#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <limits.h>

int main(int argc, char **argv)
{

    printf("CHAR_BIT = %d bits per byte\n", CHAR_BIT);
    printf("sizeof(char) = %lu bytes.\n", sizeof(char));
    printf("sizeof(int) = %lu bytes.\n", sizeof(int));
    printf("sizeof(float) = %lu bytes.\n", sizeof(float));
    printf("sizeof(double) = %lu bytes.\n", sizeof(double));
    printf("sizeof(short) = %lu bytes.\n", sizeof(short));
    printf("sizeof(long) = %lu bytes.\n", sizeof(long));
    printf("sizeof(long int) = %lu bytes.\n", sizeof(long int));
    printf("sizeof(long long) = %lu bytes.\n", sizeof(long long));
    printf("sizeof(int8_t) = %lu bytes.\n", sizeof(int8_t));
    printf("sizeof(uint8_t) = %lu bytes.\n", sizeof(uint8_t));
    printf("sizeof(uint16_t) = %lu bytes.\n", sizeof(uint16_t));
    printf("sizeof(uint32_t) = %lu bytes.\n", sizeof(uint32_t));
    printf("sizeof(char *) = %lu bytes.\n", sizeof(char *));
    printf("sizeof(int *) = %lu bytes.\n", sizeof(int *));
    printf("sizeof(float *) = %lu bytes.\n", sizeof(float *));
    printf("sizeof(void *) = %lu bytes.\n", sizeof(void *));
    printf("sizeof(void **) = %lu bytes.\n", sizeof(void **));
    printf("sizeof(int8_t *) = %lu bytes.\n", sizeof(int8_t *));
    printf("sizeof(int16_t *) = %lu bytes.\n", sizeof(int16_t *));
    printf("sizeof(int32_t *) = %lu bytes.\n", sizeof(int32_t *));
    printf("sizeof(size_t) = %lu bytes.\n", sizeof(size_t));

    return 0;
}
