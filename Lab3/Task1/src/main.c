#include "functions.h"
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc != 1) {
        printf("Ошибка: программа не принимает аргументов. \n");
        return 1;
    }

    char result[33];
    unsigned int number = 2048;

    printf("r = 1: ");
    switch (completeTask(number, 1, result)) {
        case (SUCCESS):
            printf("%s\n", result);
            printf("Успешное выполнение функции. \n");
            break;
        case(ERROR_INVALID_POINTER):
            printf("Ошибка: функции был передан невалидный указатель. \n");
            break;        
        case(ERROR_INVALID_BASE):
            printf("Ошибка: неверный ввод основния системы счисления. \n");
            break;
    }

    printf("r = 2: ");
    completeTask(number, 2, result);
    printf("%s\n", result);
    
    printf("r = 3: ");
    completeTask(number, 3, result);
    printf("%s\n", result);
    
    printf("r = 4: ");
    completeTask(number, 4, result);
    printf("%s\n", result);
    
    printf("r = 5: ");
    completeTask(number, 5, result);
    printf("%s\n", result);
    
    printf("r = 2: ");
    completeTask(0, 2, result);
    printf("%s\n", result);

    return 0;
}