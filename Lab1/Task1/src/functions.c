#include "functions.h"
#include <stdio.h>
#include <ctype.h>

unsigned int abs(int x) {
    return (x < 0) ? -x : x;
}

short int strToInt(const char* firstChar, const int base, int* result) {
    int trueNumber = 0;
    const char* curChar = firstChar;
    
    short int sign = 1;
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

        if (trueNumber > ((2147483647 - digit) / base)) {
            return 3;
        }

        trueNumber = trueNumber * base + digit;
        curChar++;
    }

    *result = trueNumber * sign;
    return 0;
}

short int findKratnie(const int x) { // Функция для пункта "a"
    if (x == 0) {
        printf("Невозможно найти делители нуля. \n");
        return 1;
    }

    if (abs(x) > 100) {
        printf("Чисел от 1 до 100, кратных %d нет. \n", x);
        return 2;
    }

    printf("Числа от 1 до 100, кратные %d: ", x);

    for (int i = abs(x); i <= 100; i += abs(x)) {
        printf("%d ", i);
    }

    printf("\n");
    return 0;
}

short int checkPrime(const int x) { // Функция для пункта "b"
    if (abs(x) <= 1) {
        return 2;
    }

    for (int i = 2; i < (x / 2); i++) {
        if (x % i == 0) {
            return 1;
        }   
    }

    return 0;
}

short int spellHex(const int x) { // Функция для пункта "c"

    printf("Результат: ");

    if (x == 0) {
        printf("0 \n");
        return 1;
    }
;
    short unsigned int digit;
    char buf[32];
    unsigned int number = abs(x);
    short unsigned int count = 0;

    while (number) {
        digit = number % 16;
        buf[count] = (digit < 10) ? '0' + digit : 'A' + digit - 10;
        number /= 16;
        count++;
    }

    if (x < 0) {
        printf("- ");
    }

    for (int i = count - 1; i >= 0; i--) {
        printf("%c ", buf[i]);
    }
    
    printf("\n");
    return 0;
}