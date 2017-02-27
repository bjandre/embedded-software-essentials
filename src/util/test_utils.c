#include <stdint.h>
#include <stdio.h>

#include "test_memory.h"
#include "test_data.h"
#include "test_circular_buffer.h"
#include "test_bit-operations.h"

int main(int argc, char **argv)
{
    int status = 0;
    status += suite_memory();
    status += suite_data();
    status += suite_circular_buffer();
    status += suite_bit_operations();

    return status;
}

