#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>
#include "functions.h"

int main(int argc, char* argv[]) {

    // Проверка вызова
    
    if (argc < 3) {
        printf("Ошибка ввода: неверное количество аргументов. Ожидается ввод в формате: \"./main -[флаг] [числа]\". \n");
        return 1;
    }

    char* flag = argv[1];
    if (flag[0] != '-' && flag[0] != '/') {
        printf("Ошибка ввода: флаг должен начинаться с '-' или '/'. \n");
        return 1;
    }

    // Решение конкретной задачи
    
    if (flag[1] == 'q' || flag[1] == 't') { // Флаги -q / -t
        if (argc != 6) {
            printf("Ошибка ввода: флаг -%c требует 4 числа для работы. \n", flag[1]);
            return 1;
        }

        double epsilon, a, b, c, temp;

        switch (strToDouble(argv[2], &epsilon)) {
            case ERROR_INVALID_INPUT:
                printf("Ошибка ввода: неверный ввод значения эпсилон. \n");
                return 1;
            case ERROR_INF_OR_NAN_APPEARED:
                printf("Ошибка ввода: inf/nan не являются вещественными числами. \n");
                return 1;
            case ERROR_INVALID_POINTER:
                printf("Ошибка функции: указатель не является валидным. \n");
                return 1;
            default:
                break;
        }
        if (epsilon < 0) {
            printf("Ошибка ввода: значение эпсилон не может быть отрицательным. \n");
            return 1;
        }

        switch (strToDouble(argv[3], &a)) {
            case ERROR_INVALID_INPUT:
                printf("Ошибка ввода: неверный ввод значения первого коэффициента. \n");
                return 1;
            case ERROR_INF_OR_NAN_APPEARED:
                printf("Ошибка ввода: inf/nan не являются вещественными числами. \n");
                return 1;
            case ERROR_INVALID_POINTER:
                printf("Ошибка функции: указатель не является валидным. \n");
                return 1;
            default:
                break;
        }
        switch (strToDouble(argv[4], &b)) {
            case ERROR_INVALID_INPUT:
                printf("Ошибка ввода: неверный ввод значения второго коэффициента. \n");
                return 1;
            case ERROR_INF_OR_NAN_APPEARED:
                printf("Ошибка ввода: inf/nan не являются вещественными числами. \n");
                return 1;
            case ERROR_INVALID_POINTER:
                printf("Ошибка функции: указатель не является валидным. \n");
                return 1;
            default:
                break;
        }
        switch (strToDouble(argv[5], &c)) {
            case ERROR_INVALID_INPUT:
                printf("Ошибка ввода: неверный ввод значения третьего коэффициента. \n");
                return 1;
            case ERROR_INF_OR_NAN_APPEARED:
                printf("Ошибка ввода: inf/nan не являются вещественными числами. \n");
                return 1;
            case ERROR_INVALID_POINTER:
                printf("Ошибка функции: указатель не является валидным. \n");
                return 1;
            default:
                break;
        }
        
        if (flag[1] == 'q') { // Флаг -q
            double perestanovki[6][3] = {};
            short int kolvoPerestanovok = 0;
            double otv1, otv2;
            for (short int k = 0; k < 2; k++) {
                for (short int i = 0; i < 3; i++) {
                    bool flag = 0;
                    for (int check; check < kolvoPerestanovok; check++) {
                        if ((perestanovki[check][0] - a < epsilon) & (perestanovki[check][1] - b < epsilon) & (perestanovki[check][2] - c < epsilon)) {
                            flag = 1;
                            break;
                        }
                    }
                    if (flag == 1) {
                        continue;
                    }
                    perestanovki[kolvoPerestanovok][0] = a;
                    perestanovki[kolvoPerestanovok][1] = b;
                    perestanovki[kolvoPerestanovok][2] = c;
                    kolvoPerestanovok++;
                    
                    printf("\n Случай a = %f, b = %f, c = %f. \n", a, b, c);
                    switch (solveEquation(epsilon, a, b, c, &otv1, &otv2)) {
                        case SUCCESS:
                            printf("Ответ: два решения - %f и %f. \n", otv1, otv2);
                            break;
                        case INF_ROOTS:
                            printf("Ответ: бесконечно много решений. \n");
                            break;
                        case NO_ROOTS:
                            printf("Ответ: нет решений. \n");
                            break;
                        case NOT_A_SQUARE_EQUATION:
                            printf("Ответ: уравнение не является квадратным. Решение единственно и равно %f. \n", otv1);
                            break;
                        case NEGATIVE_DISCRIMINANT:
                            printf("Ответ: дискриминант отрицательный, у уравнения нет действительных корней. \n");
                            break;
                        case ERROR_INVALID_POINTER:
                            printf("Ошибка функции: указатели не являются валидными. \n");
                            return 1;
                        default:
                            break;
                    }
                    temp = a;
                    a = b;
                    b = c;
                    c = temp;
                }
                temp = c;
                c = b;
                b = temp;
            }

        } else { // Флаг -t

            short int ans;
            switch (checkTriangle(epsilon, a, b, c, &ans)) {
                case SUCCESS:
                    if (!ans) {
                        printf("Числа могут являться длинами сторон прямоугольного треугольника \n");
                    } else {
                        printf("Числа не могут являться длинами сторон прямоугольного треугольника \n");
                    }
                    break;
                case ERROR_INVALID_ARGUMENTS:
                    printf("Длины сторон треугольника должны быть положительными числами. \n");
                    break;
                case ERROR_INVALID_POINTER:
                    printf("Ошибка функции: указатели не являются валидными. \n");
                    return 1;
                case ERROR_OVERFLOW:
                    printf("Ошибка функции: невозможно проверить прямоугольность треугольника из-за переполнения ячейки памяти. \n");
                    return 1;
                default:
                    break;
            }
        }

    } else if (flag[1] == 'm'){ // Флаг -m

        if (argc != 4) {
            printf("Ошибка ввода: флаг -m требует 2 числа для работы. \n");
            return 1;
        }
        int x;
        int y;
        switch (strToInt(argv[2], 10, &x))
        {
            case ERROR_NOTHING_AFTER_MINUS:
                printf("Ошибка ввода числа: после знака минуса нет цифр. \n");
                return 1;
            case ERROR_INVALID_LETTER:
                printf("Ошибка ввода числа: недопустимый символ в числе. \n");
                return 1;
            case ERROR_OVERFLOW:
                printf("Ошибка ввода числа: число переполняет ячейку памяти. \n");
                return 1;
            case ERROR_INVALID_POINTER:
                printf("Ошибка функции: указатель не является валидным. \n");
                return 1;
            default:
                break;
        }
        switch (strToInt(argv[3], 10, &y))
        {
            case ERROR_NOTHING_AFTER_MINUS:
                printf("Ошибка ввода числа: после знака минуса нет цифр. \n");
                return 1;
            case ERROR_INVALID_LETTER:
                printf("Ошибка ввода числа: недопустимый символ в числе. \n");
                return 1;
            case ERROR_OVERFLOW:
                printf("Ошибка ввода числа: число переполняет ячейку памяти. \n");
                return 1;
            case ERROR_INVALID_POINTER:
                printf("Ошибка функции: указатель не является валидным. \n");
                return 1;
            default:
                break;
        }

        short int otv;

        switch (checkKratnost(x, y, &otv)) {
            case SUCCESS:
                if (!otv) {
                    printf("Первое число кратно второму. \n");
                } else {
                    printf("Первое число не кратно второму. \n");
                }
                break;
            case ERROR_INVALID_ARGUMENTS:
                printf("Ошибка ввода чисел: для флага необходимы ненулевые числа. \n");
                return 1;
            case ERROR_INVALID_POINTER:
                printf("Ошибка функции: указатель не является валидным. \n");
                return 1;
            default:
                break;
        }

        return 0;

    } else {
        printf("Ошибка ввода: проверьте написание флага. \n");
        return 1;
    }

    return 0;
}