#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>

typedef enum {
    SUCCESS = 0,
    ERROR_INVALID_FUNCTION_ARGUMENTS,
    ERROR_CANT_FIND_PRIME_NUMBERS,
    ERROR_INVALID_INPUT
} ErrorCode;

ErrorCode clearBuffer();
ErrorCode max(const int* numbers, const int size, int* result);

ErrorCode generatePrimeNumbers(const int maxIndex, int* result);

#endif