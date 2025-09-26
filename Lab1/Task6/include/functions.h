#ifndef FUNCTIONS_H
#define FUNCTIONS_H

// Общая функция

short int solve(double (*func)(double),
    double a, double b, double epsilon,
    double* result);

// Функции интегралов

double firstIntegral(double x);

#endif