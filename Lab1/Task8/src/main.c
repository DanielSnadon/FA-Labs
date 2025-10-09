#include "functions.h"
#include <string.h>
#include <stdbool.h>

 int main(int argc, char* argv[]) {
    short int base;
    printf("Введите основание системы счисления: ");

    if (scanf("%d", &base) != 1 || base < 2 || base > 36) {
        printf("Ошибка: неверное основание системы счисления. \n");
        return 1;
    }

    if (clearBuffer()) {
        printf("Ошибка: неверный ввод. \n");
        return 1;
    }

    printf("Вводите числа, каждое с новой строки. Завершите ввод строкой \"Stop\". \n");

    const short int BUFSIZE = 8096;
    char buff[BUFSIZE];
    long long bestNumber = 0;
    bool isInputStarted = false;

    while (1) {
        if (fgets(buff, BUFSIZE - 1, stdin) == NULL) {
            printf("Ошибка чтения ввода: неверный ввод строки. Возможно строка слишком большая или пустая? \n");
            return 1;
        }

        if (!strcmp(buff, "Stop\n") || !strcmp(buff, "Stop")) {
            if (!isInputStarted) {
                printf("Ошибка ввода: закончен ввод, но числа переданы не были. \n");
                return 1;
            }
            break;
        }

        isInputStarted = true;

        switch (translate(buff, base, &bestNumber)) {
            case SUCCESS:
                break;
            case ERROR_NULL_POINTER:
                printf("Ошибка: укзатель, полученный в функции не является валидным. \n");
                return 1;
            case ERROR_EMPTY_LINE:
                printf("Ошибка: введена строка, не содержащая числа. \n");
                return 1;
            case ERROR_INVALID_INPUT:
                printf("Ошибка: в строке введён недопустимый символ. \n");
                return 1;
            case ERROR_OVERFLOW:
                printf("Ошибка: было введено число, переполняющее ячейку памяти. \n");
                return 1;
        }
    }

    int bases[] = {base, 9, 18, 27, 36, -1};
    char number[BUFSIZE];

    short int i = 0;
    for (int i = 0; bases[i] != -1; i++) {
        switch (rebase(bestNumber, bases[i], number, BUFSIZE)) {
            case SUCCESS:
                break;
            case ERROR_NULL_POINTER:
                printf("Ошибка: укзатель, полученный в функции не является валидным. \n");
                return 1;
            case ERROR_INVALID_INPUT:
                printf("Ошибка: функция перевода в другую систему счисления получила неверное значение ввода. \n");
                return 1;
            case ERROR_OVERFLOW:
                printf("Ошибка: переполнение ячейки памяти во время перевода в другую систему счисления. \n");
                return 1;
        }
        
        printf("Основание - %d: %s", bases[i], number);

        if (bases[i + 1] != -1) {
            printf("\n");
        }
    }

    printf("\n");
    return 0;
}