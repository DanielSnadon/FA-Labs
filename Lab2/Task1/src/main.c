#include "functions.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc != 1) {
        printf("Ошибка: программа не принимает аргументов. \n");
        return 1;
    }

    // Ввести свои значения тут

    long long amountOfNums = 2; // Количество чисел
    int base = 2; // Основание
    
    // // //

    bool* result = (bool*)calloc(amountOfNums, sizeof(bool));
    if (result == NULL) {
        printf ("Ошибка: не удалось выделить необходимую память. \n");
        return 1;
    }

    // Ввести свои значения тут

    ErrorCode answer = func(base, result, amountOfNums, 0.1);

    // // //

    switch (answer)
    {
    case SUCCESS:
        printf("Функция выполнилась успешно. Результат:\n");
        for (int i = 0; i < amountOfNums; i++) {
            printf("Число №%d - ", i + 1);
            if (result[i]) {
                printf("имеет конечное представление в системе счисления %d.\n", base);
            } else {
                printf("не имеет конечное представление в системе счисления %d.\n", base);
            }
        }
        break;
    case ERROR_BAD_BASE:
        printf("Ошибка: введено некорректное основание системы счисления. \n");
        break;
    case ERROR_BAD_AMOUNT_OF_NUMBERS:
        printf("Ошибка: введено некорректное значение переменной количества чисел. \n");
        break;
    case ERROR_BAD_NUMBER:
        printf("Ошибка: введено неверное значение числа. Числа должны находиться в промежутке [0, 1]. \n");
        break;
    default:
        break;
    }

    return 0;
}