#include <stdint.h>
#include <stdio.h>

#include "test_memory.h"
#include "test_data.h"


int main(int argc, char **argv)
{
    int status = 0;
    status += suite_memory();
    status += suite_data();
    return status;
}

