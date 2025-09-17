#include <stdio.h>
#include "functions.h"

int main(int argc, char* argv[]) {

    // Проверка вызова

    if (argc != 3) {
        printf("Ошибка ввода: неверное количество аргументов. Ожидается ввод в формате: \"./main [number] -[flag]\". \n");
        return 1;
    }

    char* flag = argv[2];
    if (flag[0] != '-' && flag[0] != '/') {
        printf("Ошибка ввода: флаг должен начинаться с '-' или '/'. \n");
        return 1;
    }

    // Перевод из строки в число

    int error = 0;
    int number;
    error = strToInt(argv[1], 10, &number);
    switch (error)
    {
    case 1:
        printf("Ошибка ввода числа: после знака минуса нет цифр. \n");
        return 1;
    case 2:
        printf("Ошибка ввода числа: недопустимый символ в числе. \n");
        return 1;
    case 3:
        printf("Ошибка ввода числа: число слишком большое. \n");
        return 1;
    default:
        break;
    }

    // Решение конкретной задачи

    switch (flag[1])
    {
    case 'h':
        findKratnie(number);
        break;

    case 'p':
        switch(checkPrime(number))
        {
            case 0:
                printf("%d - простое число. \n", number);
                break;
            case 1:
                printf("%d - составное число. \n", number);
                break;
            default:
                printf("%d - не является простым или составным числом. \n", number);
                break;
        }
        break;
    
    default:
        printf("Ошибка ввода: проверьте написание флага. \n");
        return 1;
        break;
    }
    
    return 0;
}