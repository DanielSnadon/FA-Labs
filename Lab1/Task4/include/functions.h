#ifndef FUNCTIONS_H
#define FUNCTIONS_H

typedef enum {
    SUCCESS = 0,
    ERROR_INVALID_POINTER,
    ERROR_BAD_CALCULATIONS,
    ERROR_LOCAL_FUNCTION_ERROR,
    ERROR_INVALID_BISECTION_ARRAY,
    ERROR_NEGATIVE_EPSILON
} ErrorCode;

// Вспомогательные функции:

ErrorCode clearBuffer();
double eFunc(const double x);
double piFunc(const double x);
double ln2Func(const double x);
double sqrt2Func(const double x);
double gammaFunc(const double x);

// Основные функции:

ErrorCode solveEquationBisection(double (*func)(double),
const double epsilon, double* result,
double a, double b);

ErrorCode solveEquationBinary(double (*func)(double),
const double epsilon, double* result,
double a, double b);

ErrorCode eLimit(const double epsilon, double* result);
ErrorCode eSeries(const double epsilon, double* result);

ErrorCode piLimit(const double epsilon, double* result);
ErrorCode piSeries(const double epsilon, double* result);

ErrorCode ln2Limit(const double epsilon, double* result);
ErrorCode ln2Series(const double epsilon, double* result);

ErrorCode sqrt2Limit(const double epsilon, double* result);
ErrorCode sqrt2Series(const double epsilon, double* result);

ErrorCode gammaLimit(const double epsilon, double* result);
ErrorCode gammaSeries(const double epsilon, double* result);

#endif