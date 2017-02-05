#include <stdio.h>
#include <stdint.h>

#include "projects.h"

int main(int argc, char **argv)
{
    printf("Hello, world!\n");

    uint32_t const project = PROJECT;

    if (project == 1) {
        project_1_report();
    }

    return 0;
}
