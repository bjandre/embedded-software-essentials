#ifndef ESE_MAIN_ASYNC_GLOBAL_H_
#define ESE_MAIN_ASYNC_GLOBAL_H_

#include <stdbool.h>

/**

  Central location to declare all asynchronous global variables

 */

typedef struct AsynchronousData {
    bool data_available;
} async_data_t;

#endif // ESE_MAIN_ASYNC_GLOBAL_H_
