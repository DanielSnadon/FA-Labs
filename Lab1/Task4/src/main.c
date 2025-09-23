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
    printf("1 - предел \n");
    printf("2 - ряд/произведение \n");
    printf("3 - уравнение \n");

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

    double answer;

    switch(chosenConst) {
        case 1: 
            switch(chosenMethod) { // Вычисление e
                case 1:
                    switch(eLimit(epsilon, &answer)) {
                        case 0:
                            printf("Результат - %f \n", answer);
                            break;
                        case 1:
                            printf("Ошибка функции: указатель не является валидным. \n");
                            return 1;
                        case 2:
                            printf("Не удалось вычислить значение с заданной точностью. \n");
                            printf("Ближайший результат - %f \n", answer);
                            return 1;
                        default:
                            break;
                    }
                    break;

                case 2:
                    switch(eSeries(epsilon, &answer)) {
                        case 0:
                            printf("Результат - %f \n", answer);
                            break;
                        case 1:
                            printf("Ошибка функции: указатель не является валидным. \n");
                            return 1;
                        case 2:
                            printf("Не удалось вычислить значение с заданной точностью. \n");
                            printf("Ближайший результат - %f \n", answer);
                            return 1;
                        case 3:
                            printf("Ошибка локальной функции: возможно отрицательный факториал или не валидный указатель? \n");
                            return 1;
                        default:
                            break;
                    }
                    break;

                case 3:
                    double a = 2.0;
                    double b = 3.0;
                    switch(solveEquationBisection(eFunc, epsilon, &answer, a, b)) {
                        case 0:
                            printf("Результат - %f \n", answer);
                            break;
                        case 1:
                            printf("Ошибка функции: указатель не является валидным. \n");
                            return 1;
                        case 2:
                            printf("Ошибка вычисления функции: неверно выбран интервал для бисекции. \n");
                            return 1;
                        default:
                            break;
                    }
                default:
                    break;
            }
            break;

        case 2: 
            switch(chosenMethod) { // Вычисление π
                case 1:
                    switch(piLimit(epsilon, &answer)) {
                        case 0:
                            printf("Результат - %f \n", answer);
                            break;
                        case 1:
                            printf("Ошибка функции: указатель не является валидным. \n");
                            return 1;
                        case 2:
                            printf("Не удалось вычислить значение с заданной точностью. \n");
                            printf("Ближайший результат - %f \n", answer);
                            return 1;
                        case 3:
                            printf("Ошибка локальной функции: возможно отрицательный факториал или не валидный указатель? \n");
                            return 1;
                        default:
                            break;
                    }
                    break;

                case 2:
                    switch(piSeries(epsilon, &answer)) {
                        case 0:
                            printf("Результат - %f \n", answer);
                            break;
                        case 1:
                            printf("Ошибка функции: указатель не является валидным. \n");
                            return 1;
                        case 2:
                            printf("Не удалось вычислить значение с заданной точностью. \n");
                            printf("Ближайший результат - %f \n", answer);
                            return 1;
                        default:
                            break;
                    }
                    break;

                case 3:
                    double a = 3.0;
                    double b = 4.0;
                    switch(solveEquationBinary(piFunc, epsilon, &answer, a, b)) {
                        case 0:
                            printf("Результат - %f \n", answer);
                            break;
                        case 1:
                            printf("Ошибка функции: указатель не является валидным. \n");
                            return 1;
                        default:
                            break;
                    }
                default:
                    break;
            }
            break;
            
        break;
        case 3:
        break;
        case 4:
        break;
        case 5:
        break;
        default:
            break;
    }
    
    return 0;
}