#ifndef FUNCTIONS_H
#define FUNCTIONS_H

short int solve(double (*func)(double),
    double a, double b, double epsilon,
    double* result);

#endif