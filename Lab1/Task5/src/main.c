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

    char *flag = argv[1];

    if (flag[0] == '-' || flag[0] == '/') {

        if (flag[1] == 'n' && argc == 4) {
            strcpy(outputFileName, argv[3]);
            
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
    
    if (inputFile == NULL) {
        perror("Ошибка открытия выходного файла. \n");
        fclose(inputFile);
        return 1;
    }
    
    char chosenFlag = flag[1] == 'n' ? flag[2] : flag[1];

    switch (chosenFlag) {
        case 'd':
            digitRemove(inputFile, outputFile);
            break;
        case 'i':
            letterCount(inputFile, outputFile);
            break;
        case 's':
            countEveryoneElse(inputFile, outputFile);
            break;
        case 'a':

            break;
        default:
            printf("Ошибка: проверьте написание значения флага. \n");
            fclose(inputFile);
            fclose(outputFile);
            return 1;
    }




    fclose(inputFile);
    fclose(outputFile);

    printf("Программа завершена успешна. Результат записан в файл. \n");
    return 0;
}