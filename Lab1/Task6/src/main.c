#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "functions.h"

int main(int argc, char* argv[]) {

    double epsilon;

    printf("Введите точность вычисления: \n");
    if (scanf("%lf", &epsilon) != 1 || epsilon < 0) {
        printf("Ошибка ввода: проверьте правильность ввода точности вычисления. \n");
        return 1;
    }
    
    double answer;

    switch (solve(firstIntegral, 0, 1, epsilon, &answer)) {
        case 0:
            printf("Значение интегралов вычислено успешно. Ответ: %f \n", answer);
            break;
        case 1:
            printf("Ошибка функции: неверное значение эпсилон. \n");
            return 1;
        case 2:
            printf("Ошибка функции: указатель не является валидным. \n");
            return 1;
        case 3:
            printf("Ошибка записи интеграла: пределы интегрирования заданы неверно. \n");
            return 1;
        case 4:
            printf("Ошибка функции: переполнение ячейки памяти на этапе начальных вычисленийю \n");
            return 1;
        case 5:
            printf("Не удалось узнать значение интеграла заданной точности. Ближайшее значение: %f \n", answer);
        

    }

    return 0;
}