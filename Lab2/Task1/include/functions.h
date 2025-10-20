#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdbool.h>

typedef enum {
    SUCCESS = 0,
    ERROR_BAD_BASE,
    ERROR_BAD_AMOUNT_OF_NUMBERS,
    ERROR_INVALID_INPUT,
    ERROR_BAD_NUMBER


} ErrorCode;

ErrorCode func(const int base, 
    bool* result, 
    const int amountOfNumbers, ...);

ErrorCode clearBuffer();

#endif

