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
    ERROR_INVALID_VALUE_IN_FILE,
    ERROR_OVERFLOW,
    ERROR_NULL_POINTER,
    ERROR_NO_DIGITS_AFTER_MINUS,
    ERROR_NOT_ALLOWED_SYMBOL,
    ERROR_OUTPUT_FILE_ERROR,
    ERROR_CANT_WRITE_ERROR,
} ErrorCode;

// Вспомогательные функции:

ErrorCode isThatFileGood(const char *name);

// Основные функции:

ErrorCode func(FILE *input, FILE *output);

#endif