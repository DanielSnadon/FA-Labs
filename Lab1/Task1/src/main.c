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

    int number;
    switch (strToInt(argv[1], 10, &number))
    {
        case 1:
            printf("Ошибка ввода числа: после знака минуса нет цифр. \n");
            return 1;
        case 2:
            printf("Ошибка ввода числа: недопустимый символ в числе. \n");
            return 1;
        case 3:
            printf("Ошибка ввода числа: число переполняет ячейку памяти. \n");
            return 1;
        default:
            break;
    }

    // Решение конкретной задачи
    
    switch (flag[1])
    {
        case 'h':
            switch (findKratnie(number))
            {
                case 1:
                    printf("Невозможно найти делители нуля. \n");
                    break;
                case 2:
                    printf("Чисел от 1 до 100, кратных %d нет. \n", number);
                    break;
                default:
                    break;
            }
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
        
        case 's':
            spellHex(number);
            break;
            
        case 'e':
            if (powerTable(number)) {
                printf("Ошибка ввода: число должно находиться в промежутке между 1 и 10. \n");
                return 1;
            }
            break;

        case 'a':
            long int result;
            switch (numberSum(number, &result))
            {
                case 1:
                    printf("Ошибка ввода: число должно быть натуральным. \n");
                    return 1;
                case 2:
                    printf("Ошибка ввода условия: результат переполняет ячейку памяти. \n");
                    return 1;
                default:
                    printf("Сумма всех натуральных чисел от 1 до %d - %ld \n", number, result);
                    break;
            }
            break;

        case 'f':
            long long int answer;
            switch (factorial(number, &answer))
            {
                case 1:
                    printf("Ошибка ввода: число должно быть неотрицательным. \n");
                    return 1;
                case 2:
                    printf("Ошибка ввода условия: результат переполняет ячейку памяти. \n");
                    return 1;
                default:
                    printf("Факториал числа %d - %lld \n", number, answer);
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