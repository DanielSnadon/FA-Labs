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
    ERROR_INVALID_OUTPUT,
    ERROR_MEMORY_ALLOCATION
} ErrorCode;

// Вспомогательные функции:

ErrorCode clearBuffer();
ErrorCode isThatFileGood(const char *name);

// Основная функция:

ErrorCode completeTask(FILE *input, FILE *output);

#endif