#include <stdio.h>

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "memory.h"
#include "data.h"

#include "test_data.h"

void test_big_to_little_null_ptr(void **state);
void test_big_to_little_valid(void **state);

void test_little_to_big_null_ptr(void **state);
void test_little_to_big_valid(void **state);

void test_convert_endian32_1(void **state);
void test_convert_endian32_2(void **state);

void test_my_atoi_leading_whitespace_null_term(void **state);
void test_my_atoi_negative_null_term(void **state);

void test_my_itoa_positive_base10(void **state);
void test_my_itoa_negative_base10(void **state);
void test_my_itoa_positive_base2(void **state);
void test_my_itoa_negative_base16(void **state);

int suite_data(void)
{
    const struct CMUnitTest data_tests[] = {
        cmocka_unit_test(test_big_to_little_null_ptr),
        cmocka_unit_test(test_big_to_little_valid),
        cmocka_unit_test(test_little_to_big_null_ptr),
        cmocka_unit_test(test_little_to_big_valid),
        cmocka_unit_test(test_convert_endian32_1),
        cmocka_unit_test(test_convert_endian32_2),
        cmocka_unit_test(test_my_atoi_leading_whitespace_null_term),
        cmocka_unit_test(test_my_atoi_negative_null_term),
        cmocka_unit_test(test_my_itoa_positive_base10),
        cmocka_unit_test(test_my_itoa_negative_base10),
        cmocka_unit_test(test_my_itoa_positive_base2),
        cmocka_unit_test(test_my_itoa_negative_base16),
    };
    return cmocka_run_group_tests(data_tests, NULL, NULL);
}

void test_big_to_little_null_ptr(void **state)
{
    // test convert big to little error with null pointer
    uint32_t const length = 1;

    DataStatus status = big_to_little32(NULL, length);
    assert_int_equal(status, DataStatus_ERROR_NULL);
}

void test_big_to_little_valid(void **state)
{
    // test convert endian by round tripping data, single 8-byte int
    uint32_t const length = 1;
    uint32_t data = 0x01234567;
    uint32_t expected = 0x67452301;
    size_t num_bytes = sizeof(data);

    DataStatus status = big_to_little32(&data, length);
    assert_int_equal(status, DataStatus_SUCCESS);
    assert_memory_equal((uint8_t *)&expected, (uint8_t *)&data, length * num_bytes);
}

void test_little_to_big_null_ptr(void **state)
{
    // test convert big to little error with null pointer
    uint32_t const length = 1;

    DataStatus status = big_to_little32(NULL, length);
    assert_int_equal(status, DataStatus_ERROR_NULL);
}

void test_little_to_big_valid(void **state)
{
    // test convert endian by round tripping data, single 8-byte int
    uint32_t const length = 1;
    uint32_t data = 0x67452301;
    uint32_t expected = 0x01234567;
    size_t num_bytes = sizeof(data);

    DataStatus status = little_to_big32(&data, length);
    assert_int_equal(status, DataStatus_SUCCESS);
    assert_memory_equal((uint8_t *)&expected, (uint8_t *)&data,
                        length * num_bytes);
}

void test_convert_endian32_1(void **state)
{
    // test convert endian by round tripping data, single 8-byte int
    uint32_t const length = 1;
    uint32_t data = 0x01234567;
    uint32_t expected = 0x67452301;
    size_t num_bytes = sizeof(data);

    // note: just calling the wrapper to avoid exposing convert_endian
    DataStatus status = big_to_little32(&data, length);
    assert_int_equal(status, DataStatus_SUCCESS);
    assert_memory_equal((uint8_t *)&expected, (uint8_t *)&data, length * num_bytes);

    uint32_t expected_orig = 0x01234567;
    status = little_to_big32(&data, length);
    assert_int_equal(status, DataStatus_SUCCESS);
    assert_memory_equal((uint8_t *)&expected_orig, (uint8_t *)&data,
                        length * num_bytes);

}

void test_convert_endian32_2(void **state)
{
    // test convert endian, array of 4 8-byte ints
    uint32_t const length = 4;
    uint32_t data[length];
    uint32_t expected[length];
    size_t num_bytes = sizeof(*data);
    *data = 0x01234567;
    *expected = 0x67452301;
    *(data + 1) = 0x89abcdef;
    *(expected + 1) = 0xefcdab89;
    *(data + 2) = 0x01020304;
    *(expected + 2) = 0x04030201;
    *(data + 3) = 0x05060708;
    *(expected + 3) = 0x08070605;

    uint32_t expected_orig[length];
    for (uint32_t i = 0; i < length; i++) {
        *(expected_orig + i) = *(data + i);
    }

    // note: just calling the wrapper to avoid exposing convert_endian
    DataStatus status = little_to_big32(data, length);
    assert_int_equal(status, DataStatus_SUCCESS);
    assert_memory_equal((uint8_t *)expected, (uint8_t *)data, length * num_bytes);

    status = big_to_little32(data, length);
    assert_int_equal(status, DataStatus_SUCCESS);
    assert_memory_equal((uint8_t *)expected_orig, (uint8_t *)data,
                        length * num_bytes);

}

void test_my_atoi_leading_whitespace_null_term(void **state)
{
    // test converting a null terminated string with leading whitespace.
#define LENGTH 10
    int8_t string[LENGTH] = "     1002\0";
    uint32_t expected = 1002;
    uint32_t received = my_atoi(string);
#   ifdef NDEBUG
    if (received != expected) {
        printf("ERROR: 0x%.2x =? 0x%.2x\n", received, expected);
    }
#   endif
    assert_int_equal(expected, received);
#undef LENGTH
}

void test_my_atoi_negative_null_term(void **state)
{
    // test converting a negative number as a null terminated string
#define LENGTH 10
    int8_t string[LENGTH] = " -1537902\0";
    uint32_t expected = -1537902;
    uint32_t received = my_atoi(string);
#   ifdef NDEBUG
    if (received != expected) {
        printf("ERROR: 0x%.2x =? 0x%.2x\n", received, expected);
    }
#   endif
    assert_int_equal(expected, received);
#undef LENGTH
}

void test_my_itoa_positive_base10(void **state)
{
    // test converting an integer base 10 to a string
#define LENGTH 33
    uint32_t length = LENGTH;
    int8_t string[LENGTH];
    int8_t expected[LENGTH];
    for (uint8_t i = 0; i < length; i++) {
        string[i] = '\0';
        expected[i] = string[i];
    }
    my_memmove((uint8_t *)"1023", (uint8_t *)expected, 4);
    uint32_t data = 1023;
    uint32_t base = 10;
    int8_t *junk = my_itoa(string, data, base);

    assert_ptr_equal(junk, string);
    assert_memory_equal((uint8_t *)string, (uint8_t *)expected, length);
#undef LENGTH
}

void test_my_itoa_negative_base10(void **state)
{
    // test converting an negative integer base 10 to a string
#define LENGTH 33
    uint32_t length = LENGTH;
    int8_t string[LENGTH];
    int8_t expected[LENGTH];
    for (uint8_t i = 0; i < length; i++) {
        string[i] = '\0';
        expected[i] = string[i];
    }
    my_memmove((uint8_t *)"-10325476", (uint8_t *)expected, 9);
    uint32_t data = -10325476;
    uint32_t base = 10;
    int8_t *junk = my_itoa(string, data, base);

    assert_ptr_equal(junk, string);
    assert_memory_equal((uint8_t *)string, (uint8_t *)expected, length);
#undef LENGTH
}


void test_my_itoa_positive_base2(void **state)
{
    // test converting an integer base 2 to a string
#define LENGTH 33
    uint32_t length = LENGTH;
    int8_t string[LENGTH];
    int8_t expected[LENGTH];
    for (uint8_t i = 0; i < length; i++) {
        string[i] = '\0';
        expected[i] = string[i];
    }
    my_memmove((uint8_t *)"1111110101", (uint8_t *)expected, 10);

    uint32_t data = 1013;
    uint32_t base = 2;
    int8_t *junk = my_itoa(string, data, base);

    assert_ptr_equal(junk, string);
    assert_memory_equal((uint8_t *)string, (uint8_t *)expected, length);
#undef LENGTH
}

void test_my_itoa_negative_base16(void **state)
{
    // test converting a negative integer base 16 to a string
#define LENGTH 33
    uint32_t length = LENGTH;
    int8_t string[LENGTH];
    int8_t expected[LENGTH];
    for (uint8_t i = 0; i < length; i++) {
        string[i] = '\0';
        expected[i] = string[i];
    }
    my_memmove((uint8_t *)"-9d8de4", (uint8_t *)expected, 7);

    uint32_t data = -10325476;
    uint32_t base = 16;
    int8_t *junk = my_itoa(string, data, base);

    assert_ptr_equal(junk, string);
    assert_memory_equal((uint8_t *)string, (uint8_t *)expected, length);
#undef LENGTH
}
