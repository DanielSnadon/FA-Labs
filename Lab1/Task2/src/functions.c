#include "functions.h"
#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>

short int clearBuffer() {
    char c;
    while ((c = getchar()) != '\n' && c != '\0')
    {
    }
    return 0;
}

short int max(const int* numbers, const int size, int* result) {
    if (numbers == NULL || result == NULL || size < 0) {
        return 1;
    }

    int maxNumber = numbers[0];
    for (int i = 1; i < size; i++) {
        if (numbers[i] > maxNumber) {
            maxNumber = numbers[i];
        }
    }

    *result = maxNumber;
    return 0;
}

short int generatePrimeNumbers(const int maxIndex, int* primeNumbers) {
    if (maxIndex <= 0 || primeNumbers == NULL) {
        return 1;
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
        return 1;
    }
    
    return 0;
}