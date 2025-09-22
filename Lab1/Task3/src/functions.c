#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>

double doubleAbs(double x) {
    return (x < 0) ? -x : x;
}

int abs(int x) {
    return (x < 0) ? -x : x;
}

bool isEquals(const double epsilon, const double a, const double b) {
    return doubleAbs(a - b) < epsilon;
}

short int strToInt(const char* firstChar, const int base, int* result) {
    if (result == NULL || firstChar == NULL) {
        return 4;
    }

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

short int strToDouble(const char* firstChar, double* result) {
    if (firstChar == NULL || result == NULL) {
        return 3;
    }
    
    char* remainingChars;
    *result = strtod(firstChar, &remainingChars);

    if (*remainingChars != '\0' || remainingChars == firstChar) {
        return 1;
    }
    
    if (isnan(*result) || isinf(*result)) {
        return 2;
    }

    return 0;
}

short int solveEquation(const double epsilon,
    const double a, const double b, const double c,
    double* otv1, double* otv2) {
    if (otv1 == NULL || otv2 == NULL) {
        return 5;
    }

    if (isEquals(epsilon, a, 0.0)) {
        if (isEquals(epsilon, b, 0.0)) {
            if (isEquals(epsilon, c, 0.0)) {
                return 1;
            }
            return 2;
        }
        *otv1 = -c / b;
        return 3;
    }

    double d = b * b - 4 * a * c;

    if (isEquals(epsilon, d, 0.0)) {
        *otv1 = -b / (2 * a);
        *otv2 = *otv1;
        return 0;
    } else if (d > 0) {
        *otv1 = (-b + sqrt(d)) / (2 * a);
        *otv2 = (-b - sqrt(d)) / (2 * a);
        return 0;
    } else {
        return 4;
    }
}

short int checkKratnost(const int a, const int b, short int *otv) {
    if (otv == NULL) {
        return 2;
    }

    if (a == 0 || b == 0) {
        return 1;
    }

    if (abs(a) % abs(b) == 0) {
        *otv = 0;
    }
    else {
        *otv = 1;
    }

    return 0;
}

short int checkTriangle(const double epsilon,
    double a, const double b, const double c, short int *otv) {
    if (a <= 0 || b <= 0 || c <= 0) {
        return 1;
    }
    if (otv == NULL) {
        return 2;
    }
    if ((a > sqrt(__DBL_MAX__)) || (b > sqrt(__DBL_MAX__)) || (c > sqrt(__DBL_MAX__))) {
        return 3;
    }
    if (doubleAbs(a * a + b * b - c * c) < epsilon || doubleAbs(c * c + b * b - a * a) < epsilon || doubleAbs(a * a + c * c - b * b) < epsilon) {
        *otv = 0;
    } else {
        *otv = 1;
    }

    return 0;
}