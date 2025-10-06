#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>
#include "functions.h"

int main(int argc, char* argv[]) {
    short int chosenConst;

    printf("Выберите константу: \n");
    printf("1 - e \n");
    printf("2 - π \n");
    printf("3 - ln2 \n");
    printf("4 - √2 \n");
    printf("5 - γ \n");

    if (scanf("%hd", &chosenConst) != 1 || chosenConst < 1 || chosenConst > 5) {
        printf("Ошибка ввода: проверьте правильность ввода выбранного значения константы. \n");
        return 1;
    }
    clearBuffer();

    short int chosenMethod;

    printf("Выберите способ вычисления: \n");
    printf("1 - Предел \n");
    printf("2 - Ряд/Произведение \n");
    printf("3 - Уравнение \n");

    if (scanf("%hd", &chosenMethod) != 1 || chosenMethod < 1 || chosenMethod > 3) {
        printf("Ошибка ввода: проверьте правильность ввода выбранного значения метода вычисления. \n");
        return 1;
    }
    clearBuffer();
    
    double epsilon;

    printf("Введите точность вычисления: \n");
    if (scanf("%lf", &epsilon) != 1 || epsilon < 0) {
        printf("Ошибка ввода: проверьте правильность ввода точности вычисления. \n");
        return 1;
    }

    short int errorNumber = 0;
    double answer;

    switch(chosenConst) { // Таблица
        case 1: 
            switch(chosenMethod) { // Вычисление e
                case 1:
                    errorNumber = eLimit(epsilon, &answer);
                    break;

                case 2:
                    errorNumber = eSeries(epsilon, &answer);
                    break;

                case 3:
                    double a = 2.0;
                    double b = 3.0;
                    errorNumber = solveEquationBisection(eFunc, epsilon, &answer, a, b);
                    break;
            }
            break;

        case 2: 
            switch(chosenMethod) { // Вычисление π
                case 1:
                    errorNumber = piLimit(epsilon, &answer);
                    break;

                case 2:
                    errorNumber = piSeries(epsilon, &answer);
                    break;

                case 3:
                    double a = 3.0;
                    double b = 4.0;
                    errorNumber = solveEquationBinary(piFunc, epsilon, &answer, a, b);
                    break;
            }
            break;
        
        case 3:
            switch(chosenMethod) { // Вычисление ln2
                case 1:
                    errorNumber = ln2Limit(epsilon, &answer);
                    break;

                case 2:
                    errorNumber = ln2Series(epsilon, &answer);
                    break;

                case 3:
                    double a = 0.0;
                    double b = 1.0;
                    errorNumber = solveEquationBisection(ln2Func, epsilon, &answer, a, b);
                    break;
            }
            break;
            
        case 4:
            switch(chosenMethod) { // Вычисление sqrt(2)
                case 1:
                    errorNumber = sqrt2Limit(epsilon, &answer);
                    break;

                case 2:
                    errorNumber = sqrt2Series(epsilon, &answer);
                    break;

                case 3:
                    double a = 1.0;
                    double b = 2.0;
                    errorNumber = solveEquationBisection(sqrt2Func, epsilon, &answer, a, b);
            }
            break;

        case 5:
            switch(chosenMethod) { // Вычисление γ
                case 1:
                    errorNumber = gammaLimit(epsilon, &answer);
                    break;

                case 2:
                    errorNumber = gammaSeries(epsilon, &answer);
                    break;

                case 3:
                    double a = -100.0;
                    double b = 100.0;
                    errorNumber = solveEquationBisection(gammaFunc, epsilon, &answer, a, b);
            }
            break;

        default:
            break;
    }

    switch (errorNumber) {
        case SUCCESS:
            printf("Результат - %f \n", answer);
            return 0;
        case ERROR_INVALID_POINTER:
            printf("Ошибка функции: указатель не является валидным. \n");
            return 1;
        case ERROR_BAD_CALCULATIONS:
            printf("Не удалось вычислить значение с заданной точностью. \n");
            printf("Ближайший результат - %f \n", answer);
            return 0;
        case ERROR_LOCAL_FUNCTION_ERROR:
            printf("Ошибка локальной функции: возможно отрицательный факториал или не валидный указатель? \n");
            return 1;
        case ERROR_INVALID_BISECTION_ARRAY:
            printf("Ошибка вычисления функции: неверно выбран интервал для бисекции. \n");
            return 1;
        case ERROR_NEGATIVE_EPSILON:
            printf("Ошибка значения эпсилон: эпсилон не может быть отрицательным. \n");
            return 1;   
        default:
            break;
    }
    
    return 1;
}

