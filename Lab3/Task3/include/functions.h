#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "linked_list.h"

typedef enum {
    SUCCESS = 0,
    ERROR_INVALID_FILE_POINTER,
    ERROR_NO_FILE_TYPE,
    UNKNOWN_FILE_TYPE,
} ErrorCode;

ErrorCode isThatFileGood(const char *name);




#endif