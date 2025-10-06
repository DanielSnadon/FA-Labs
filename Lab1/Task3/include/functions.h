#ifndef FUNCTIONS_H
#define FUNCTIONS_H

typedef enum {
    SUCCESS = 0,
    NEGATIVE_DISCRIMINANT,
    NOT_A_SQUARE_EQUATION,
    NO_ROOTS,
    INF_ROOTS,
    ERROR_INVALID_INPUT,
    ERROR_INF_OR_NAN_APPEARED,
    ERROR_INVALID_POINTER,
    ERROR_OVERFLOW,
    ERROR_INVALID_ARGUMENTS,
    ERROR_INVALID_LETTER,
    ERROR_NOTHING_AFTER_MINUS

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