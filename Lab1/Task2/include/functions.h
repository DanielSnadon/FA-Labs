#ifndef FUNCTIONS_H
#define FUNCTIONS_H

typedef enum {
    SUCCESS = 0,
    ERROR_INVALID_FUNCTION_ARGUMENTS,
    ERROR_CANT_FIND_PRIME_NUMBERS
} ErrorCode;

ErrorCode clearBuffer();
ErrorCode max(const int* numbers, const int size, int* result);

ErrorCode generatePrimeNumbers(const int maxIndex, int* result);

#endif