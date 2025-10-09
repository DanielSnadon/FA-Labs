#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "functions.h"

int main(int argc, char* argv[]) {
    
    if (argc != 2) {
        printf("Ошибка ввода: неверное количество аргументов. Ожидается ввод в формате: \"./main [точность]\". \n");
        return 1;
    }

    double epsilon;
    
    switch (strToDouble(argv[1], &epsilon)) {
        case ERROR_INVALID_INPUT:
            printf("Ошибка ввода: неверный ввод значения эпсилон. \n");
            return 1;
        case ERROR_INF_OR_NAN_APPEARED:
            printf("Ошибка ввода: inf/nan не являются вещественными числами. \n");
            return 1;
        case ERROR_INVALID_POINTER:
            printf("Ошибка функции: указатель не является валидным. \n");
            return 1;
        default:
            break;
    }

    if (epsilon < 0) {
        printf("Ошибка ввода: значение эпсилон не может быть отрицательным. \n");
        return 1;
    }

    short int chosenIntegral;

    printf("Выберите интеграл: \n");
    printf("1 - ∫ ln((1+x)/x) \n");
    printf("2 - ∫ e^(-(x^2)/2) \n");
    printf("3 - ∫ ln(1/(1-x)) \n");
    printf("4 - ∫ x^x \n");

    if (scanf("%hd", &chosenIntegral) != 1 || chosenIntegral < 1 || chosenIntegral > 4) {
        printf("Ошибка ввода: проверьте правильность ввода номера выбранного интеграла. \n");
        return 1;
    }

    double answer;
    ErrorCode status;

    switch (chosenIntegral) {
        case 1:
            status = solve(firstIntegral, 0, 1, epsilon, &answer);
            break;
        case 2:
            status = solve(secondIntegral, 0, 1, epsilon, &answer);
            break;
        case 3:
        /*
            Интеграл расходится при значениях, бесконечно близких к 1.
            Поскольку нужно вычислить точное значение,
            то будем использовать конечное приближение к единице.
        */
            status = solve(thirdIntegral, 0, 1 - 1e-16, epsilon, &answer);
            break;
        case 4:
            status = solve(fourthIntegral, 0, 1, epsilon, &answer);
            break;
    }

    switch (status) {
        case SUCCESS:
            printf("Значение интегралов вычислено успешно. Ответ: %f \n", answer);
            break;
        case ERROR_BAD_EPSILON:
            printf("Ошибка функции: неверное значение эпсилон. \n");
            return 1;
        case ERROR_INVALID_POINTER:
            printf("Ошибка функции: указатель не является валидным. \n");
            return 1;
        case ERROR_INVALID_INTEGRAL_LIMITS:
            printf("Ошибка записи интеграла: пределы интегрирования заданы неверно. \n");
            return 1;
        case ERROR_OVERFLOW:
            printf("Ошибка функции: переполнение ячейки памяти на этапе начальных вычислений. \n");
            return 1;
        case ERROR_BAD_PRECISION_ANSWER:
            printf("Не удалось узнать значение интеграла заданной точности. Ближайшее значение: %f \n", answer);
            break;
    }

    return 0;
}