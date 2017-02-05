#ifndef PROJECT
#define PROJECT -1
#endif

#if PROJECT == 1
#    include "project_1.h"
#else
#    error "Unsupported project number 'PROJECT'. Valid values: 1"
#endif
