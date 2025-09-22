#include "functions.h"
#include <stdio.h>
#include <ctype.h>
#include <limits.h>

unsigned int abs(int x) {
    return (x < 0) ? -x : x;
}

long power(const int base, const int exponent) {
    long result = 1;
    for (int i = 0; i < exponent; i++) {
        result *= base;
    }

    return result;
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

        if (trueNumber > ((INT_MAX - digit) / base)) {
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
        return 1;
    }

    if (abs(x) > 100) {
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
    if (x <= 1) {
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

short int powerTable(const int x) { // Функция для пункта "d"
    if (x > 10 || x <= 1) {
        return 1;
    }

    printf("< Таблица степеней от 1 до %d для чисел 1-10 > \n\n", x);

    for (int number = 0; number <= 10; number++) {

        printf("Для %d: \n", number);

        for (int exp = 1; exp <= x; exp++) {
            long result = power(number, exp);
            printf("%d ^ %d = %ld \n", number, exp, result);
        }

        printf("\n");
    }

    return 0;
}

short int numberSum(const int x, long* result) { // Функция для пункта "e"
    if (x <= 0) {
        return 1;
    }

    long int sum = 0;

    for (int i = 1; i <= x; i++) {
        if (LONG_MAX - i < sum) {
            return 2;
        }
        sum += i;
    }

    *result = sum;

    return 0;
}

short int factorial(const int x, long long* result) { // Функция для пункта "f"
    if (x < 0) {
        return 1;
    }

    long long temp = 1;

    for (int i = 1; i <= x; i++) {
        if (temp > LLONG_MAX / i) {
            return 2;
        }
        temp *= i;
    }

    *result = temp;
    
    return 0;
}