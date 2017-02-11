#include <stdio.h>
#include <stdint.h>

#if PROJECT == 1
#    include "project_1.h"
#else
#    error Unsupported project number in PROJECT macro. Valid values: 1
#endif

int main(int argc, char **argv)
{
    printf("Hello, world!\n");

#   if PROJECT == 1
        project_1_report();
#   endif

    return 0;
}
