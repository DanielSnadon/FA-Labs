#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Локальные функции

short int clearBuffer() {
    char c;
    while ((c = getchar()) != '\n' && c != '\0')
    {
    }
    return 0;
}

short int factorial(const int x, double* result) {
    if (result == NULL || x < 0) {
        return 1;
    }

    double temp = 1;

    for (int i = 1; i <= x; i++) {
        if (temp > __DBL_MAX__ / i) {
            return 2;
        }
        temp *= i;
    }

    *result = temp;
    
    return 0;
}

// Общие функции решения уравнений

short int solveEquationBisection(double (*func)(double),
const double epsilon, double* result,
double a, double b) {
    if (result == NULL || func == NULL) {
        return 1;
    }

    if (func(a) * func(b) > 0) {
        return 2;
    }

    double middle;

    while ((b - a) > epsilon) {
        middle = (a + b) / 2.0;
        
        if (fabs(func(middle)) < epsilon) {
            *result = middle;
            return 0;
        }

        if (func(a) * func(middle) < 0) {
            b = middle;
        } else {
            a = middle;
        }
    }

    *result = middle;

    return 0;
}


short int solveEquationBinary(double (*func)(double),
const double epsilon, double* result,
double a, double b) {
    if (result == NULL || func == NULL) {
        return 1;
    }

    double middle;

    while ((b - a) > epsilon) {
        middle = (a + b) / 2.0;
        
        if (fabs(func(middle)) < epsilon) {
            *result = middle;
            return 0;
        }

        if (func(a) < func(b)) {
            b = middle;
        } else {
            a = middle;
        }
    }

    *result = middle;

    return 0;
}
// Функции для e

short int eLimit(const double epsilon, double* result) {
    if (result == NULL) {
        return 1;
    }

    double curr, prev = 2.0;
    short int flag = 1;

    for (int n = 1; __INT_MAX__ / 2 > n; n*=2) {

        prev = curr;
        curr = pow(1.0 + 1.0/n, n);

        if (fabs(curr - prev) < epsilon) {
            *result = curr;
            return 0;
        }
    }

    *result = curr;

    return 2;
}

short int eSeries(const double epsilon, double* result) {
    if (result == NULL) {
        return 1;
    }

    double prev = 0.0;
    double curr = 0.0;

    for (int n = 0; __INT_MAX__ - 1 > n; n++) {

        prev = curr;
        double factorN;
        switch (factorial(n, &factorN)) {
            case 1:
                return 3;
            case 2:
                *result = curr;
                return 2;
            default:
                break;
        }
        curr += 1 / factorN;


        if (fabs(curr - prev) < epsilon) {
            *result = curr;
            return 0;
        }
    }

    *result = curr;

    return 2;
}

double eFunc(const double x) {
    return log(x) - 1.0;
}

// Функции для pi

short int piLimit(const double epsilon, double* result) {
    if (result == NULL) {
        return 1;
    }

    double curr, prev = 2.0;
    short int flag = 1;

    for (int n = 1; __INT_MAX__ / 2 > n; n*=2) {

        double factorN;
        switch (factorial(n, &factorN)) {
            case 1:
                return 3;
            case 2:
                *result = prev;
                return 2;
            default:
                break;
        }

        double dbFactorN;
        switch (factorial(2 * n, &dbFactorN)) {
            case 1:
                return 3;
            case 2:
                *result = prev;
                return 2;
            default:
                break;
        }

        prev = curr;
        curr = pow((pow(2.0, n) * factorN), 4) / (n * pow(dbFactorN, 2));

        if (fabs(curr - prev) < epsilon) {
            *result = curr;
            return 0;
        }
    }

    *result = curr;

    return 2;
}

short int piSeries(const double epsilon, double* result) {
    if (result == NULL) {
        return 1;
    }

    double prev = 1.0;
    double curr = 1.0;

    for (int n = 1; __INT_MAX__ / 2 > n; n++) {

        prev = curr;

        curr += (n % 2 == 0) ? 1.0/(2*n + 1) : -1.0/(2*n + 1);

        if (fabs(4.0 * curr - 4.0 * prev) < epsilon) {
            *result = curr * 4.0;
            return 0;
        }
    }

    *result = 4.0 * prev;

    return 2;
}

double piFunc(const double x) {
    return cos(x) + 1.0;
}

