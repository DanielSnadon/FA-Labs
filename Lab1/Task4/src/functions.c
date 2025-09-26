#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

// Локальные функции

bool isPrime(int x)
{
    if (x <= 1) {
        return false;
    }
    if (x <= 3) {
        return true;
    }
    if (x % 2 == 0 || x % 3 == 0) {
        return false;
    }

    for (int i = 5; i * i <= x; x += 2) {
        if (x % i == 0) {
            return false;
        }
    }

    return true;
}

short int clearBuffer()
{
    char c;
    while ((c = getchar()) != '\n' && c != '\0')
    {
    }
    return 0;
}

double factorial(const int x)
{
    double result = 1;
    
    for (int i = 1; i <= x; i++) {
        if (result > __DBL_MAX__ / i) {
            return -1;
        }
        result *= i;
    }

    return result;
}

double combinationsNumber(int m, int k)
{
    double factorM = factorial(m);

    if (factorM < 0) {
        return -1;
    }

    double factorK = factorial(k);

    if (factorM < 0) {
        return -1;
    }

    double factorR = factorial(m - k);

    if (__DBL_MAX__ / factorK < factorR) {
        return -1;
    }

    return factorM / (factorK * factorR);
}

// Общие функции решения уравнений

short int solveEquationBisection(double (*func)(double),
const double epsilon, double* result,
double a, double b)
{
    if (epsilon < 0) {
        return 5;
    }

    if (result == NULL || func == NULL) {
        return 1;
    }

    if (func(a) * func(b) > 0) {
        return 4;
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
double a, double b)
{
    if (epsilon < 0) {
        return 5;
    }

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

short int eLimit(const double epsilon, double* result)
{
    if (epsilon < 0) {
        return 5;
    }

    if (result == NULL) {
        return 1;
    }

    double prev, curr = 2.0;
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

short int eSeries(const double epsilon, double* result)
{
    if (epsilon < 0) {
        return 5;
    }

    if (result == NULL) {
        return 1;
    }

    double prev, curr = 0.0;

    for (int n = 0; __INT_MAX__ - 1 > n; n++) {

        prev = curr;

        double factorN = factorial(n);
        if (factorN < 0) {
            *result = curr;
            return 2;
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

double eFunc(const double x)
{
    return log(x) - 1.0;
}

// Функции для pi

short int piLimit(const double epsilon, double* result)
{
    if (epsilon < 0) {
        return 5;
    }

    if (result == NULL) {
        return 1;
    }

    double prev, curr = 3.0;
    short int flag = 1;

    for (int n = 1; __INT_MAX__ / 2 > n; n*=2) {

        double factorN = factorial(n);
        if (factorial < 0) {
            *result = prev;
            return 2;
        }

        double dbFactorN = factorial(2 * n);
        if (factorial < 0) {
            *result = prev;
            return 2;
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

short int piSeries(const double epsilon, double* result)
{
    if (epsilon < 0) {
        return 5;
    }

    if (result == NULL) {
        return 1;
    }

    double prev, curr = 0.0;

    for (int n = 1; __INT_MAX__ / 2 > n; n++) {

        prev = curr;

        curr += (pow(-1.0, n - 1.0) / (2.0 * n - 1.0));

        if (fabs(4.0 * curr - 4.0 * prev) < epsilon) {
            *result = curr * 4.0;
            return 0;
        }
    }

    *result = 4.0 * prev;

    return 2;
}

double piFunc(const double x)
{
    return cos(x) + 1.0;
}

// Функции для ln2

short int ln2Limit(const double epsilon, double* result)
{
    if (epsilon < 0) {
        return 5;
    }

    if (result == NULL) {
        return 1;
    }

    double prev, curr = 0.0;

    short int flag = 1;

    for (int n = 1; __INT_MAX__ / 2 > n; n*=2) {

        prev = curr;

        curr = n * (pow(2, 1.0 / n) - 1);

        if (fabs(curr - prev) < epsilon) {
            *result = curr;
            return 0;
        }
    }

    *result = curr;

    return 2;
}

short int ln2Series(const double epsilon, double* result)
{
    if (epsilon < 0) {
        return 5;
    }

    if (result == NULL) {
        return 1;
    }

    double prev, curr = 0.0;

    for (int n = 1; __INT_MAX__ - 1 > n; n++) {

        prev = curr;

        curr += (pow(-1.0, n - 1.0) / n);

        if (fabs(curr - prev) < epsilon) {
            *result = curr;
            return 0;
        }
    }

    *result = prev;

    return 2;
}

double ln2Func(const double x)
{
    return exp(x) - 2.0;
}

// Функции для sqrt2

short int sqrt2Limit(const double epsilon, double* result)
{
    if (epsilon < 0) {
        return 5;
    }

    if (result == NULL) {
        return 1;
    }

    double prev, curr = -0.5;

    short int flag = 1;

    while (__DBL_MAX__ / fabs(curr) > fabs(curr)) {

        prev = curr;

        curr = curr - (curr * curr) / 2.0 + 1.0;

        if (fabs(curr - prev) < epsilon) {
            *result = curr;
            return 0;
        }
    }

    *result = curr;

    return 2;
}

short int sqrt2Series(const double epsilon, double* result)
{
    if (epsilon < 0) {
        return 5;
    }
    
    if (result == NULL) {
        return 1;
    }

    double prev, curr = 1.0;

    for (int k = 2; __INT_MAX__ - 1 > k; k++) {

        prev = curr;

        curr *= pow(2.0, pow(2.0, -k));

        if (fabs(curr - prev) < epsilon) {
            *result = curr;
            return 0;
        }
    }

    *result = prev;

    return 2;
}

double sqrt2Func(const double x)
{
    return pow(x, 2.0) - 2.0;
}

// Функции для γ

short int gammaLimit(const double epsilon, double* result)
{
    if (epsilon < 0) {
        return 5;
    }

    if (result == NULL) {
        return 1;
    }

    double prev, curr = 0.0;

    double minimalRaz = 10.0;
    double closestRez = curr;
    
    for (int m = 10; __INT_MAX__ - 1 > m; m+=1) {

        prev = curr;
        curr = 0.0;

        for (int k = 1; k <= m; k++) {
            double combNum = combinationsNumber(m, k);

            if (combNum < 0) {
                *result = closestRez;
                return 2;
            }

            double cof = pow(-1.0, k) / k;
            double logNum = log(factorial(k));
            double temp = cof * combNum;

            if (__DBL_MAX__ / fabs(temp) < fabs(logNum)) {
                *result = closestRez;
                return 2;
            }

            temp *= logNum;

            if (__DBL_MAX__ - curr < temp) {
                *result = closestRez;
                return 2;
            }
            
            curr += temp;
        }

        if (fabs(curr - prev) < epsilon) {
            *result = curr;
            return 0;
        }

        if (fabs(curr - prev) < minimalRaz) {
            minimalRaz = fabs(curr - prev);
            closestRez = curr;
        }
    }

    *result = closestRez;

    return 2;
}

short int gammaSeries(const double epsilon, double* result) // НЕ РАБОТАЕТ
{ 
    if (epsilon < 0) {
        return 5;
    }
    
    if (result == NULL) {
        return 1;
    }

    const double pi = 3.14159265358979323846;

    double prev, curr = 0.0;

    for (int k = 2; __INT_MAX__ - 1 > k; k++) {

        prev = curr;

        double temp = 1.0/sqrt(k * k) - 1.0/k;

        curr += temp;

        if (fabs(curr - prev) < epsilon) {
            *result = curr - (pi * pi) / 6;
            return 0;
        }
    }

    *result = prev - (pi * pi) / 6;

    return 2;
}

double gammaFunc(const double x)
{

    double result;

    const int t = 1000000;

    double sum = 1.0;

    for (int p = 2; p <= t; p++) {

        if (isPrime(p)) {
            sum *= ((p - 1.0)/p);
        }
    }

    // printf("%f * %f\n", log(t), sum);

    /*
    Почему такая неточность?
    
    Разбор вручную:
    
    Попробуем разные значения t, используя функцию вывода выше.

    Заметим, что меняя значение t мы меняем значение ln(t),
    а sum ~= const ~= 0.16, так как
    в определённый момент домножение на значение,
    близкое к единице становится бесполезным.
    
    ln(t) тем временем будет принимать значения, зависящие от
    максимального t. Если предел
    стремится к бесконечности, то выставим t = INT_MAX и получим
    ln(INT_MAX) ~= 21 соответственно.

    Выполняем умножение 21 * 0.16 ~= 3.36

    И тогда получаем уравнение: exp(-x) = 3.36

    Вычисляем вручную и получаем x ~= -1.21

    Что в сравнении с константой эйлера (0.5772) находится
    достаточно далеко.

    Скорее всего проблема заключается в неточности double,
    в лимитах чисел и вычислительных способностях компьютера.

    P.S. Хотя если рассматривать в промежутке от -100 до 100,
    то значение найдено относительно точно ;)
    */

    result = log(__INT_MAX__) * sum;

    return exp(-x) - result;
}