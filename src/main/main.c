#include <stdio.h>
#include <stdint.h>

#if PROJECT == 0
#    // NOTE(bja, 2017-02): Building main with no project code.
#elif PROJECT == 1
#    include "project_1.h"
#elif PROJECT == 2
#
#else
#    error "Unsupported project number in PROJECT macro. Valid values: 0, 1"
#endif

int main(int argc, char **argv)
{
    printf("Hello, world!\n");

#   if PROJECT == 1
    project_1_report();
#   elif PROJECT == 2
#
#   endif

    return 0;
}
