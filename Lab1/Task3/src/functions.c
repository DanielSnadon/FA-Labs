#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>

double doubleAbs(double x)
{
    return (x < 0) ? -x : x;
}

int abs(int x)
{
    return (x < 0) ? -x : x;
}

bool isEquals(const double epsilon, const double a, const double b)
{
    return doubleAbs(a - b) < epsilon;
}

ErrorCode strToInt(const char* firstChar, const int base, int* result)
{
    if (result == NULL || firstChar == NULL) {
        return ERROR_INVALID_POINTER;
    }

    int trueNumber = 0;
    const char* curChar = firstChar;
    
    short int sign = 1;
    if (*firstChar == '-') {
        sign = -1;
        curChar++;
    }

    if (*curChar == '\0') {
        return ERROR_NOTHING_AFTER_MINUS;
    }
    while (*curChar) {
        char c = *curChar;
        int digit;
        
        if (isdigit(c)) {
            digit = c - '0';
        } else if (isalpha(c)) {
            digit = c - 'A' + 10;
        } else {
            return ERROR_INVALID_LETTER;
        }
        
        if (digit >= base) {
            return ERROR_INVALID_LETTER;
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

ErrorCode strToDouble(const char* firstChar, double* result)
{
    if (firstChar == NULL || result == NULL) {
        return ERROR_INVALID_POINTER;
    }
    
    char* remainingChars;
    *result = strtod(firstChar, &remainingChars);

    if (*remainingChars != '\0' || remainingChars == firstChar) {
        return ERROR_INVALID_INPUT;
    }
    
    if (isnan(*result) || isinf(*result)) {
        return ERROR_INF_OR_NAN_APPEARED;
    }

    return SUCCESS;
}

ErrorCode solveEquation(const double epsilon,
    const double a, const double b, const double c,
    double* otv1, double* otv2)
{
    if (otv1 == NULL || otv2 == NULL) {
        return ERROR_INVALID_POINTER;
    }

    if (isEquals(epsilon, a, 0.0)) {
        if (isEquals(epsilon, b, 0.0)) {
            if (isEquals(epsilon, c, 0.0)) {
                return INF_ROOTS;
            }
            return NO_ROOTS;
        }
        *otv1 = -c / b;
        return NOT_A_SQUARE_EQUATION;
    }

    double d = b * b - 4 * a * c;

    if (isEquals(epsilon, d, 0.0)) {
        *otv1 = -b / (2 * a);
        *otv2 = *otv1;
        return SUCCESS;
    } else if (d > 0) {
        *otv1 = (-b + sqrt(d)) / (2 * a);
        *otv2 = (-b - sqrt(d)) / (2 * a);
        return SUCCESS;
    } else {
        return NEGATIVE_DISCRIMINANT;
    }
}

ErrorCode checkKratnost(const int a, const int b, short int *otv) 
{
    if (otv == NULL) {
        return ERROR_INVALID_POINTER;
    }

    if (a == 0 || b == 0) {
        return ERROR_INVALID_ARGUMENTS;
    }

    if (abs(a) % abs(b) == 0) {
        *otv = 0;
    }
    else {
        *otv = 1;
    }

    return SUCCESS;
}

ErrorCode checkTriangle(const double epsilon,
    double a, const double b, const double c, short int *otv)
{
    if (a <= 0 || b <= 0 || c <= 0) {
        return ERROR_INVALID_ARGUMENTS;
    }
    if (otv == NULL) {
        return ERROR_INVALID_POINTER;
    }
    if ((a > sqrt(__DBL_MAX__)) || (b > sqrt(__DBL_MAX__)) || (c > sqrt(__DBL_MAX__))
    || (a * a > (__DBL_MAX__ - b * b)) || (b * b > (__DBL_MAX__ - c * c)) || (c * c > (__DBL_MAX__ - a * a))) {
        return ERROR_OVERFLOW;
    }
    if (doubleAbs(a * a + b * b - c * c) < epsilon || doubleAbs(c * c + b * b - a * a) < epsilon || doubleAbs(a * a + c * c - b * b) < epsilon) {
        *otv = 0;
    } else {
        *otv = 1;
    }

    return SUCCESS;
}