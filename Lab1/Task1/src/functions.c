#include "functions.h"
#include <stdio.h>
#include <ctype.h>
#include <limits.h>

// Вспомогательные функции:

unsigned int abs(int x)
{
    return (x < 0) ? -x : x;
}

long power(const int base, const int exponent)
{
    long result = 1;
    for (int i = 0; i < exponent; i++) {
        result *= base;
    }

    return result;
}

ErrorCode strToInt(const char* firstChar, const int base, int* result)
{
    if (firstChar == NULL || result == NULL) {
        return ERROR_NULL_POINTER;
    }

    int trueNumber = 0;
    const char* curChar = firstChar;
    
    short int sign = 1;
    if (*firstChar == '-') {
        sign = -1;
        curChar++;
    }

    if (*curChar == '\0') {
        return ERROR_NO_DIGITS_AFTER_MINUS;
    }
    while (*curChar) {
        char c = *curChar;
        short int digit;
        
        if (isdigit(c)) {
            digit = c - '0';
        } else if (isalpha(c)) {
            digit = c - 'A' + 10;
        } else {
            return 2;
        }
        
        if (digit >= base) {
            return ERROR_NOT_ALLOWED_SYMBOL;
        }

        if (trueNumber > ((INT_MAX - digit) / base)) {
            return ERROR_OVERFLOW;
        }

        trueNumber = trueNumber * base + digit;
        curChar++;
    }

    *result = trueNumber * sign;
    return SUCCESS;
}

ErrorCode findKratnie(const int x) // Функция для пункта "a"
{ 
    if (x == 0) {
        return ERROR_ZERO_DIVISION;
    }

    if (abs(x) > 100) {
        return ERROR_NO_NUMBERS_FOUND;
    }
    
    printf("Числа от 1 до 100, кратные %d: ", x);

    for (int i = abs(x); i <= 100; i += abs(x)) {
        printf("%d ", i);
    }

    printf("\n");
    return SUCCESS;
}

ErrorCode checkPrime(const int x) // Функция для пункта "b"
{ 
    if (x <= 1) {
        return ERROR_NOT_PRIME_OR_COMPOSITE;
    }

    for (int i = 2; i < (x / 2); i++) {
        if (x % i == 0) {
            return ALTERNATIVE_SUCCESS;
        }   
    }

    return SUCCESS;
}

ErrorCode spellHex(const int x) // Функция для пункта "c"
{ 
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

    

    printf("Результат: ");

    if (x == 0) {
        printf("0 \n");
        return SUCCESS;
    }

    if (x < 0) {
        printf("- ");
    }

    for (int i = count - 1; i >= 0; i--) {
        printf("%c ", buf[i]);
    }

    printf("\n");
    return SUCCESS;
}

ErrorCode powerTable(const int x) // Функция для пункта "d"
{ 
    if (x > 10 || x <= 1) {
        return ERROR_NUMBER_OUT_OF_RANGE;
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

    return SUCCESS;
}

ErrorCode numberSum(const int x, long* result) // Функция для пункта "e"
{ 
    if (result == NULL) {
        return ERROR_NULL_POINTER;
    }

    if (x <= 0) {
        return ERROR_NUMBER_MUST_BE_NATURAL;
    }

    long int sum = 0;

    for (int i = 1; i <= x; i++) {
        if (LONG_MAX - i < sum) {
            return ERROR_OVERFLOW;
        }
        sum += i;
    }

    *result = sum;

    return SUCCESS;
}

ErrorCode factorial(const int x, long long* result) // Функция для пункта "f"
{ 
    if (result == NULL) {
        return ERROR_NULL_POINTER;
    }

    if (x < 0) {
        return ERROR_NUMBER_CANT_BE_NEGATIVE;
    }

    long long temp = 1;

    for (int i = 1; i <= x; i++) {
        if (temp > LLONG_MAX / i) {
            return ERROR_OVERFLOW;
        }
        temp *= i;
    }

    *result = temp;
    
    return SUCCESS;
}