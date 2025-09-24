#ifndef FUNCTIONS_H
#define FUNCTIONS_H

// Вспомогательные функции:

short int clearBuffer();
double eFunc(const double x);
double piFunc(const double x);
double ln2Func(const double x);
double sqrt2Func(const double x);
double gammaFunc(const double x);

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

short int ln2Limit(const double epsilon, double* result);
short int ln2Series(const double epsilon, double* result);

short int sqrt2Limit(const double epsilon, double* result);
short int sqrt2Series(const double epsilon, double* result);

short int gammaLimit(const double epsilon, double* result);
short int gammaSeries(const double epsilon, double* result);

#endif