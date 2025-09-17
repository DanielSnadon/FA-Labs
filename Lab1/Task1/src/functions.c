#include "functions.h"
#include <stdio.h>
#include <ctype.h>

int abs(int x) {
    return (x < 0) ? -x : x;
}

int strToInt(const char* firstChar, const int base, int* result) {
    int trueNumber = 0;
    const char* curChar = firstChar;
    
    int sign = 1;
    if (*firstChar == '-') {
        sign = -1;
        curChar++;
    }

    if (*curChar == '\0') {
        return 1;
    }
    while (*curChar) {
        char c = *curChar;
        int digit;
        
        if (isdigit(c)) {
            digit = c - '0';
        } else if (isalpha(c)) {
            digit = c - 'A' + 10;
        } else {
            return 2;
        }
        
        if (digit >= base) {
            return 2;
        }

        trueNumber = trueNumber * base + digit;
        curChar++;
    }

    *result = trueNumber * sign;
    return 0;
}

void find_kratnie(const int x) {
    if (x == 0) {
        printf("Невозможно найти делители нуля. \n");
        return;
    }

    if (abs(x) > 100) {
        printf("Чисел, кратных %d нет. \n", x);
        return;
    }

    printf("Числа, кратные %d: ", x);

    for (int i = abs(x); i <= 100; i += abs(x)) {
        printf("%d ", i);
    }

    printf("\n");
    return;
}