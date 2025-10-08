#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <math.h>

typedef enum {
    SUCCESS = 0,
    ERROR_INVALID_INPUT,
    ERROR_NULL_POINTER,
    ERROR_EMPTY_LINE,
    ERROR_INVALID_CHAR,
    ERROR_OVERFLOW,
} ErrorCode;

// Вспомогательное:

ErrorCode clearBuffer();

// Основное:

ErrorCode translate(char* line, const short int base, long long* maxNumber);
ErrorCode rebase(const long long number, const short int base, char* answer, const short int maxSize);
#endif