#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>

typedef enum {
    SUCCESS = 0,
    ERROR_INVALID_FILE_POINTER,
    ERROR_NO_FILE_TYPE,
    UNKNOWN_FILE_TYPE,
    ERROR_OUTPUT_FILE_ERROR,
    ERROR_FILE_CORRUPTED
} ErrorCode;

ErrorCode digitRemove(FILE *input, FILE *output);
ErrorCode letterCount(FILE *input, FILE *output);
ErrorCode countEveryoneElse(FILE *input, FILE *output);
ErrorCode hexReplace(FILE *input, FILE *output);

ErrorCode isThatFileGood(const char *name);

#endif