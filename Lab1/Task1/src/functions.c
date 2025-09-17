#include "functions.h"
#include <stdio.h>
#include <ctype.h>

unsigned int abs(int x) {
    return (x < 0) ? -x : x;
}

unsigned short int strToInt(const char* firstChar, const int base, int* result) {
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

void findKratnie(const int x) { // Функция для пункта "a"
    if (x == 0) {
        printf("Невозможно найти делители нуля. \n");
        return;
    }

    if (abs(x) > 100) {
        printf("Чисел от 1 до 100, кратных %d нет. \n", x);
        return;
    }

    printf("Числа от 1 до 100, кратные %d: ", x);

    for (int i = abs(x); i <= 100; i += abs(x)) {
        printf("%d ", i);
    }

    printf("\n");
    return;
}

unsigned short int checkPrime(const int x) { // Функция для пункта "b"
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

void spellHex(const int x) { // Функция для пункта "c"

    printf("Результат: ");

    if (x == 0) {
        printf("0 \n");
        return;
    }
;
    short unsigned int digit;

    int number = abs(x);
    
    char buf[33];
    char* curChar = buf + 32;

    *curChar = '#';
    curChar--;

    while (number) {
        curChar--;
        digit = number % 16;
        if (digit > 9) {
            *curChar = 'A' + digit - 10;

        } else {
            *curChar = '0' + digit;
        }
        number /= 16;
    }

    if (x < 0) {
        printf("- ");
    }

    while (*curChar != '#') {
        printf("%c ", *curChar);
        curChar++;
    }

    printf("\n");
    return;
}