#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

ErrorCode solve(double (*func)(double),
    double a, double b, double epsilon,
    double* result)
{
    if (epsilon < 0) {
        return ERROR_BAD_EPSILON;
    }

    if (func == NULL || result == NULL) {
        return ERROR_INVALID_POINTER;
    }

    if (a > b) {
        return ERROR_INVALID_INTEGRAL_LIMITS;
    }

    if (a == b) {
        *result = 0;
        return SUCCESS;
    }

    if (__DBL_MAX__ - func(b) < func(a)) {
        return ERROR_OVERFLOW;
    }

    double curr, prev = -INFINITY;

    int amoutOfSteps = 1;

    unsigned int maxIters = 1e5;
    unsigned int iters = 0;

    while (iters < maxIters) {
        iters++;

        double h = (b - a) / amoutOfSteps;

        double curr = ((func(a) + func(b)) / 2);

        for (int i = 1; i < amoutOfSteps; i++) {
            double x = a + i * h;

            if (__DBL_MAX__ - curr < h * func(x)) {
                *result = prev;
                return ERROR_BAD_PRECISION_ANSWER;
            }

            curr += func(x);
        }

        curr *= h;

        if (fabs(prev - curr) < epsilon) {
            *result = curr;
            return SUCCESS;
        }

        // Проверить промежуточные результаты)
        // printf("%f VS %f \n", prev, curr);

        prev = curr;

        if (__INT_MAX__ / 2 < amoutOfSteps) {
            *result = prev;
            return ERROR_BAD_PRECISION_ANSWER;
        }

        amoutOfSteps *= 2;
    }

    *result = prev;

    return ERROR_BAD_PRECISION_ANSWER;
}

double firstIntegral(double x) {
    double y = log(1.0 + x) / x;
    if (isnan(y)) {
        return 0;
    }
    if (isinf(y)) {
        return (y > 0) ? 1e300 : 1e-300;
    }
    return y;
}

double secondIntegral(double x) {
    double y = exp(-(x * x) / 2);
    if (isnan(y)) {
        return 0;
    }
    if (isinf(y)) {
        return (y > 0) ? 1e300 : 1e-300;
    }
    return y;
}

double thirdIntegral(double x) {
    double y = -log(1.0 - x);
    if (isnan(y)) {
        return 0;
    }
    if (isinf(y)) {
        return (y > 0) ? 1e300 : -1e300;
    }
    return y;
}

/* Математическое тождество, известное как "Мечта второкурсника".
                                        Открыто И. Бернулли в 1697 году.
Вместо возведения в степень x - возведём в степень e. */
double fourthIntegral(double x) {
    double y = exp(x * log(x));
    if (isnan(y)) {
        return 0;
    }
    if (isinf(y)) {
        return (y > 0) ? 1e300 : 1e-300;
    }
    return y;
}