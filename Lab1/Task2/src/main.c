#include "functions.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    int numbersCount;
    printf("Введите количество запросов: ");

    if (scanf("%d", &numbersCount) != 1 || numbersCount < 0) {
        printf("Ошибка: неверное число запросов. \n");
        return 1;
    }
    if (clearBuffer()) {
        printf("Ошибка: неверный ввод. \n");
        return 1;
    }

    if (numbersCount == 0) {
        return 0;
    }

    int* numbers = (int*)malloc(numbersCount * sizeof(int));
    if (numbers == NULL) {
        printf ("Ошибка: не удалось выделить необходимую память. \n");
        return 1;
    }
    printf("Введите %d запросов: \n", numbersCount);

    for (int i = 0; i < numbersCount; i++) {
        if (scanf("%d", &numbers[i]) != 1 || numbers[i] <= 0) {
            printf("Ошибка: введено неверное значение запроса. \n");
            free(numbers);
            return 1;
        }
        if (clearBuffer()) {
            printf("Ошибка: неверный ввод. \n");
            return 1;
        }
    }

    int maxNumber;
    if (max(numbers, numbersCount, &maxNumber))
    {
        free(numbers);
        printf("Ошибка max: неверно введённые данные. \n");
        return 1;
    }

    int* primeNumbers = (int*)calloc(maxNumber, sizeof(int));

    if (generatePrimeNumbers(maxNumber, primeNumbers) == 1) {
        printf("Ошибка: не удалось найти простые числа. \n");
        return 1;
    }

    printf("Простые числа с соответсвующими порядковыми номерами: \n");

    for (int i = 0; i < numbersCount; i++) {
        printf("%d \n", primeNumbers[numbers[i] - 1]);
    }

    free(primeNumbers);
    free(numbers);
    return 0;
}