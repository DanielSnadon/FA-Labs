#ifndef FUNCTIONS_H
#define FUNCTIONS_H

typedef enum {
    SUCCESS = 0,

} ErrorCode;

// Вспомогательные функции:
ErrorCode strToInt(const char* firstChar, const int base, int* result);
ErrorCode strToDouble(const char* firstChar, double* result);

// Основные функции:

ErrorCode solveEquation(const double epsilon,
    const double a, const double b, const double c,
    double* otv1, double* otv2);
ErrorCode checkKratnost(const int a, const int b, short int* otv);
ErrorCode checkTriangle(const double epsilon,
    double a, const double b, const double c, short int *otv);

#endif