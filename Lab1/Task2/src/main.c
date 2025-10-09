#include "functions.h"

int main(int argc, char* argv[]) {
    if (argc != 1) {
        printf("Ошибка: программа не принимает аргументов. \n");
        return 1;
    }

    long long readedValue;
    printf("Введите количество запросов: ");

    if (scanf("%lld", &readedValue) != 1 || readedValue < 0 || readedValue > __INT_MAX__) {
        printf("Ошибка: неверное число запросов. \n");
        return 1;
    }
    if (clearBuffer()) {
        printf("Ошибка: неверный ввод, после значения количества запросов были введены лишние данные. \n");
        return 1;
    }

    int numbersCount = (int)readedValue;

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
            printf("Ошибка: неверный ввод, после значения запроса были введены лишние данные. \n");
            free(numbers);
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
        free(primeNumbers);
        free(numbers);
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