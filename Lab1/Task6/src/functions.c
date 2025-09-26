#include "functions.h"
#include <stdio.h>
#include <stdlib.h>

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
    
    if (isnan(func(b))) {
        return 4;
    }

    double razn = (b - a);

    double prev = razn * ((func(a) * func(b)) / 2);

    int amoutOfSteps = 1;

    unsigned int maxIters = 1e5;
    unsigned int iters = 0;

    while (iters < maxIters) {
        iters++;


    }
    for (abs(curr - prev) > epsilon) {
        prev = curr;

        curr += func();
    }
}