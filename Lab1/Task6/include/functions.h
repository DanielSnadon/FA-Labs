#ifndef FUNCTIONS_H
#define FUNCTIONS_H

typedef enum {
    SUCCESS = 0,
    ERROR_BAD_PRECISION_ANSWER,
    ERROR_BAD_EPSILON,
    ERROR_INVALID_POINTER,
    ERROR_INVALID_INTEGRAL_LIMITS,
    ERROR_OVERFLOW,
    ERROR_INF_OR_NAN_APPEARED,
    ERROR_INVALID_INPUT
} ErrorCode;

// Общая функция

ErrorCode solve(double (*func)(double),
    double a, double b, double epsilon,
    double* result);

// Функции интегралов

double firstIntegral(double x);
double secondIntegral(double x);
double thirdIntegral(double x);
double fourthIntegral(double x);

// Вспомогательные функции

ErrorCode strToDouble(const char* firstChar, double* result);

#endif