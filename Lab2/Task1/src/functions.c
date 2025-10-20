#include "functions.h"
#include <stdio.h>
#include <stdarg.h>

// Локальные функции

bool isPrime(long long x)
{
    if (x <= 1.0) {
        return false;
    }
    if (x <= 3.0) {
        return true;
    }
    if (x % 2 == 0 || x % 3 == 0) {
        return false;
    }

    for (long long i = 5.0; i * i <= x; i += 2.0) {
        if (x % i == 0) {
            return false;
        }
    }

    return true;
}

ErrorCode clearBuffer()
{
    char c;
    while ((c = getchar()) != '\n' && c != '\0')
    {
        if (c != ' ') {
            return ERROR_INVALID_INPUT;
        }
    }
    return SUCCESS;
}

ErrorCode func(const int base, bool* result, const int amountOfNumbers, ...) {
    if (base < 2) {
        
        return ERROR_BAD_BASE;
    }

    if (amountOfNumbers < 1) {
        return ERROR_BAD_AMOUNT_OF_NUMBERS;
    }

    va_list curr;
    double currentNumber;

    va_start(curr, amountOfNumbers);

    for (int i = 0; i < amountOfNumbers; i++) {
        currentNumber = va_arg(curr, double);

        if (currentNumber < 0 || currentNumber > 1) {
            return ERROR_BAD_NUMBER;
        }

        long long chislitel = currentNumber * 1e15;
        long long znamenatel = 1e15;

        if (currentNumber * 1e15 - (double)chislitel > 1e-15) {
            printf("Предупреждение: число №%d содержит большое количество знаков после запятой. Результат может быть неверным. \n", i + 1);
        }

        long long temp1 = chislitel;
        long long temp2 = znamenatel;

        while (temp1 && temp2) {
            if (temp1 > temp2) {
                temp1 %= temp2;
            } else {
                temp2 %= temp1;
            }
        }

        znamenatel /= (temp1 + temp2);

        if (isPrime(znamenatel) && (base % znamenatel != 0)) {
            result[i] = false;
            continue;
        }

        short int flag = 0;

        for (long scan = 1; scan < (znamenatel / 2 + 1); scan++) {
            if ((base % scan != 0) && isPrime(scan)) {
                flag = 1;
                break;
            }
        }
        if (flag == 1) {
            result[i] = false;
            continue;
        }

        result[i] = true;
    }

    va_end(curr);

    return SUCCESS;
}