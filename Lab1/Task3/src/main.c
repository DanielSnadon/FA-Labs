#include <stdio.h>
#include "functions.h"

int main(int argc, char* argv[]) {

    // Проверка вызова
    
    if (argc != 3) {
        printf("Ошибка ввода: неверное количество аргументов. Ожидается ввод в формате: \"./main [number] -[flag]\". \n");
        return 1;
    }

    char* flag = argv[1];
    if (flag[0] != '-' && flag[0] != '/') {
        printf("Ошибка ввода: флаг должен начинаться с '-' или '/'. \n");
        return 1;
    }

    // Решение конкретной задачи
    
    switch (flag[1])
    {
        case 'h':
            
    }
}