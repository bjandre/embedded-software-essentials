#ifdef BARE_METAL
#define PRINTF(X)
#else
#define PRINTF(ARGS) printf(ARGS)
#include <stdio.h>
#endif

#include <stdint.h>
#include <stdlib.h>

#include "circular_buffer.h"

//#include "ese-platform.h"
//#include "platform-generics.h"

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

#if 0
    PRINTF("Hello, from Emebbed Software Essentials Project!\n");

#if PROJECT == 1
    project_1_report();
#elif PROJECT == 2
#define junk
#endif

    size_t bytes_per_item = 1;
    size_t num_items = 32;

    CircularBuffer_t *cb_1byte = NULL;
    CircularBufferStatus status = CircularBufferNew(&cb_1byte, num_items,
                                  bytes_per_item);
    if (CB_No_Error == status) {
        // FIXME(bja, 2017-02) convert to logging infrastructure.
        PRINTF("Circular buffer initialized!\n");
    } else {
        PRINTF("Error initializing circular buffer!\n");
    }

    board_t *board = NULL;
    BoardStatus board_status = BoardInitialize((void **)(&board));

    board_status = InitializeDebugInterface(board)
                   board_status = BoardInitializePort(board, Port_B_Name);
    board_status = BoardInitializeGPIO(board, Port_B_Name);
    if (Board_OK != board_status) {
    }
    // set clock
    // set port
    // Enable the PortB clock
    // Configure Mode PB.18 to GPIO use
    board->port_b->pcr[18] = (1 << 3);
    // Set Direction PB.18 to output
    board->gpio_b->pddr |= (1 << 18);
    // Start Pin LED to On Output
    board->gpio_b->pdor |= (1 << 18);

    while (1) {
        for (size_t i = 0; i < 100000; i++) {
            // do nothing for a while.
        }
        board->gpio_b->ptor |= (1 << 18);
    }
#endif
    uint32_t i = 0;
    while (1) {
        i++;
    }
    return 0;
}
