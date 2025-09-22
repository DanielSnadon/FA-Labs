#ifndef FUNCTIONS_H
#define FUNCTIONS_H

int abs(int x);

short int strToInt(const char* firstChar, const int base, int* result);
short int strToDouble(const char* firstChar, double* result);

short int solveEquation(const double epsilon,
    const double a, const double b, const double c,
    double* otv1, double* otv2);
short int checkKratnost(const int a, const int b, short int* otv);
short int checkTriangle(const double epsilon,
    double a, const double b, const double c, short int *otv);

#endif