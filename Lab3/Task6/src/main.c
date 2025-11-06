#include "functions.h"
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc != 1) {
        printf("Ошибка: программа принимает строки в интерактивном режиме. \n");
        return 1;
    }

    char line[1024];

    printf("Вводите строки для проверки валидности расстановки скобок. (Введите \"end\" для выхода) \n");

    while (1) {
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }

        line[strcspn(line, "\n")] = '\0';

        if (strcmp(line, "end") == 0) {
            break;
        }

        if (strlen(line) == 0) {
            printf("Была введена пустая строка. \n");
            continue;
        }

        int result = check_brackets(line);

        switch (result)
        {
        case (0):
            printf("Скобки в строке расставлены правильно. \n");
            break;
        case (1):
            printf("Скобки в строке расставлены неправильно. \n");
            break;
        case (-1):
            printf("Ошибка: некорректные входные данные. \n");
            break;
        }
    }

    return 0;
}