#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    SUCCESS = 0,
    UNKNOWN_FILE_TYPE,
    ERROR_NO_FILE_TYPE,
    ERROR_INVALID_FILE_POINTER,
    ERROR_INVALID_VALUE_IN_FILE

} ErrorCode;

// Вспомогательные функции:

ErrorCode isThatFileGood(const char *name);

// Основные функции:

ErrorCode func(FILE *input, FILE *output);

#endif