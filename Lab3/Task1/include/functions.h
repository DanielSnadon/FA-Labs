#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>

typedef enum {
    SUCCESS = 0,
    ERROR_INVALID_POINTER,
    ERROR_INVALID_BASE
} ErrorCode;

ErrorCode completeTask(const unsigned int number, const int r, char *result);

#endif