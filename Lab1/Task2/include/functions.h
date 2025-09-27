#ifndef FUNCTIONS_H
#define FUNCTIONS_H

typedef enum {
    SUCCESS = 0,

} ErrorCode;

short int max(const int* numbers, const int size, int* result);

short int generatePrimeNumbers(const int maxIndex, int* result);

short int clearBuffer();

#endif