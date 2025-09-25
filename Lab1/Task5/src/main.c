#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Ошибка вызова программы: недостаточно аргументов. \nОжидается ввод в формате: \"./main [флаг] [входной файл] [выходной файл (опционально)].\" \n");
        return 1;
    }

    char *inputFileName = argv[2];
    char outputFileName[256];

    switch(isThatFileGood(inputFileName)) {
        case 1:
            printf("Ошибка функции проверки расширения файла: указатель на файл не является валидным. \n");
            break;
        case 2:
            printf("Ошибка входного файла: у входного файла отсутсвует расширение. Убедитесь, что работаете с правильным файлом. \n");
            break;
        case 3:
            printf("Ошибка входного файла: расширение файла не входит в число проверенных, результат может отличаться от ожидаемого. Продолжить? [y/n]: ");
            if (getchar() != 'y') {
                return 1;
            }
            break;
    }
    
    char *flag = argv[1];

    if (flag[0] == '-' || flag[0] == '/') {

        if (flag[1] == 'n' && argc == 4) {
            
            strcpy(outputFileName, argv[3]);

            switch(isThatFileGood(inputFileName)) {
                case 1:
                    printf("Ошибка функции проверки расширения файла: указатель на файл не является валидным. \n");
                    break;
                case 2:
                    printf("Ошибка входного файла: у входного файла отсутсвует расширение. Убедитесь, что работаете с правильным файлом. \n");
                    break;
                case 3:
                    printf("Ошибка входного файла: расширение файла не входит в число проверенных, результат может отличаться от ожидаемого. Продолжить? [y/n]: ");
                    if (getchar() != 'y') {
                        return 1;
                    }
                    break;
            }       
            
        } else {
            strcpy(outputFileName, "out_");
            strcat(outputFileName, inputFileName);
        }

    } else {
        printf("Ошибка ввода: флаг должен начинаться с '-' или '/'. \n");
        return 1;
    }

    if (strcmp(inputFileName, outputFileName) == 0) {
        printf("Ошибка: имена входного и выходного файлов совпадают, возможна потеря данных. \n");
        return 1;
    }

    FILE *inputFile = fopen(inputFileName, "r");

    if (inputFile == NULL) {
        perror("Ошибка открытия входного файла. \n");
        return 1;
    }

    FILE *outputFile = fopen(outputFileName, "w");
    
    if (outputFile == NULL) {
        perror("Ошибка открытия выходного файла. \n");
        fclose(inputFile);
        return 1;
    }
    
    char chosenFlag = flag[1] == 'n' ? flag[2] : flag[1];

    switch (chosenFlag) {
        case 'd':
            if (digitRemove(inputFile, outputFile)) {
                printf("Ошибка: указатель на файл не является валидным. \n");
                return 1;
            }
            break;
        case 'i':
            if (letterCount(inputFile, outputFile)) {
                printf("Ошибка: указатель на файл не является валидным. \n");
                return 1;
            }
            break;
        case 's':
            if (countEveryoneElse(inputFile, outputFile)) {
                printf("Ошибка: указатель на файл не является валидным. \n");
                return 1;
            }
            break;
        case 'a':
            if (hexReplace(inputFile, outputFile)) {
                printf("Ошибка: указатель на файл не является валидным. \n");
                return 1;
            }
            break;
        default:
            printf("Ошибка: проверьте написание значения флага. \n");
            fclose(inputFile);
            fclose(outputFile);
            return 1;
    }

    fclose(inputFile);
    fclose(outputFile);

    printf("Программа завершена успешно. Результат записан в файл. \n");
    return 0;
}