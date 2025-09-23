#ifndef FUNCTIONS_H
#define FUNCTIONS_H

// Вспомогательные функции:

short int clearBuffer();
double eFunc(const double x);
double piFunc(const double x);

// Основные функции:

short int solveEquationBisection(double (*func)(double),
const double epsilon, double* result,
double a, double b);

short int solveEquationBinary(double (*func)(double),
const double epsilon, double* result,
double a, double b);

short int eLimit(const double epsilon, double* result);
short int eSeries(const double epsilon, double* result);

short int piLimit(const double epsilon, double* result);
short int piSeries(const double epsilon, double* result);

#endif