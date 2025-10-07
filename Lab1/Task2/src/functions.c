#include "functions.h"
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

ErrorCode clearBuffer()
{
    char c;
    while ((c = getchar()) != '\n' && c != '\0')
    {
        if (c != ' ') {
            return ERROR_INVALID_INPUT;
        }
    }
    return SUCCESS;
}

ErrorCode max(const int* numbers, const int size, int* result)
{
    if (numbers == NULL || result == NULL || size < 0) {
        return ERROR_INVALID_FUNCTION_ARGUMENTS;
    }

    int maxNumber = numbers[0];
    for (int i = 1; i < size; i++) {
        if (numbers[i] > maxNumber) {
            maxNumber = numbers[i];
        }
    }

    *result = maxNumber;
    return SUCCESS;
}

ErrorCode generatePrimeNumbers(const int maxIndex, int* primeNumbers)
{
    if (maxIndex <= 0 || primeNumbers == NULL) {
        return ERROR_INVALID_FUNCTION_ARGUMENTS;
    }

    primeNumbers[0] = 2;

    int numberGenerated = 1, currentNumber = 1;
    bool flag;

    do {
        currentNumber += 2;
        flag = true;
        for (int p = 0; p < numberGenerated; p++) {
            if (!(currentNumber % primeNumbers[p])) {
                flag = false;
                break;
            }
        }
        if (flag) {
            primeNumbers[numberGenerated++] = currentNumber;
        }
    } while (numberGenerated != maxIndex || currentNumber == INT_MAX);

    if (numberGenerated != maxIndex) {
        return ERROR_CANT_FIND_PRIME_NUMBERS;
    }
    
    return SUCCESS;
}