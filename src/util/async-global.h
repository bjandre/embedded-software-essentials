#ifndef ESE_MAIN_ASYNC_GLOBAL_H_
#define ESE_MAIN_ASYNC_GLOBAL_H_

#include <stdbool.h>

/**

  Central location to declare all asynchronously accessed global variables

 */

typedef struct AsynchronousData {
    bool data_available; // flag indicating data is available to be retreived
    // from logger receive buffer
} async_data_t;

// FIXME(bja, 2017-03) need to add logger instance here, but there are starting
// to be circular dependencies between the libraries that need to be
// straightened out....

#endif // ESE_MAIN_ASYNC_GLOBAL_H_
