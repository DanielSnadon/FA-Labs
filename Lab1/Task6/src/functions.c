#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

short int solve(double (*func)(double),
    double a, double b, double epsilon,
    double* result)
{
    if (epsilon < 0) {
        return 1;
    }

    if (func == NULL || result == NULL) {
        return 2;
    }

    if (a > b) {
        return 3;
    }

    if (a == b) {
        *result = 0;
        return 0;
    }

    if (__DBL_MAX__ - func(b) < func(a)) {
        return 4;
    }

    double curr, prev = 0.0;

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
                return 5;
            }

            curr += func(x);
        }

        curr *= h;

        printf("%f", curr);

        if (fabs(prev - curr) < epsilon) {
            *result = curr;
            return 0;
        }

        prev = curr;
    }

    *result = prev;

    return 5;
}

double firstIntegral(double x) {
    if (isnan(log(1.0 + x) / x)) {
        return 0;
    }
    return log(1.0 + x) / x;
}