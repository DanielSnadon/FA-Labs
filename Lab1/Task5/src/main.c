#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "functions.h"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Ошибка вызова программы: недостаточно аргументов. \nОжидается ввод в формате: \"./main [флаг] [входной файл] [выходной файл (опционально)].\" \n");
        return 1;
    }

    char *inputFileName = argv[2];
    char outputFileName[256];

    switch(isThatFileGood(inputFileName)) {
        case ERROR_INVALID_FILE_POINTER:
            printf("Ошибка функции проверки расширения файла: указатель на файл не является валидным. \n");
            break;
        case ERROR_NO_FILE_TYPE:
            printf("Ошибка входного файла: у входного файла отсутсвует расширение. Убедитесь, что работаете с правильным файлом. \n");
            break;
        case UNKNOWN_FILE_TYPE:
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
                case ERROR_INVALID_FILE_POINTER:
                    printf("Ошибка функции проверки расширения файла: указатель на файл не является валидным. \n");
                    break;
                case ERROR_NO_FILE_TYPE:
                    printf("Ошибка входного файла: у входного файла отсутсвует расширение. Убедитесь, что работаете с правильным файлом. \n");
                    break;
                case UNKNOWN_FILE_TYPE:
                    printf("Ошибка входного файла: расширение файла не входит в число проверенных, результат может отличаться от ожидаемого. Продолжить? [y/n]: ");
                    if (getchar() != 'y') {
                        return 1;
                    }
                    break;
            }       
            
        } else {
            strcpy(outputFileName, "out_");
            strcat(outputFileName, inputFileName);

            if (argc == 4) {
                printf("Ошибка ввода: При вводе флага без символа \"n\" имя четвёртого файла не требуется - алгоритм задаст имя самостоятельно. Продолжить? [y/n]: ");
                if (getchar() != 'y') {
                    return 1;
                }
            }
        }

    } else {
        printf("Ошибка ввода: флаг должен начинаться с '-' или '/'. \n");
        return 1;
    }

    char* shrtInptFlNm = strrchr(inputFileName, '/');
    char* shrtOutptFlNm = strrchr(outputFileName, '/');

    shrtInptFlNm = (shrtInptFlNm != NULL) ? shrtInptFlNm + 1 : inputFileName;
    shrtOutptFlNm = (shrtOutptFlNm != NULL) ? shrtOutptFlNm + 1 : inputFileName;


    if (strcmp(shrtInptFlNm, shrtOutptFlNm) == 0) {
        printf("Ошибка: имена входного и выходного файлов совпадают, возможна потеря данных. Продолжить? [y/n]: ");
        if (getchar() != 'y') {
            return 1;
        }
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
    
    char chosenFlag = (flag[1] == 'n') ? flag[2] : flag[1];

    if (!((strlen(flag) == 2) || (flag[1] == 'n' && strlen(flag) == 3))) {
        printf("Ошибка: некорректная запись флага. \n");
        fclose(inputFile);
        fclose(outputFile);
        return 1;
    }

    bool succesful = false;

    switch (chosenFlag) {
        case 'd':
            switch (digitRemove(inputFile, outputFile)) {
                case ERROR_INVALID_FILE_POINTER:
                    printf("Ошибка: указатель на файл не является валидным. \n");
                    break;
                case ERROR_OUTPUT_FILE_ERROR:
                    printf("Ошибка: не удалось записать данные в выходной файл. \n");
                    break;
                case SUCCESS:
                    succesful = true;
                    break;
            }
            break;
        case 'i':
            switch (letterCount(inputFile, outputFile)) {
                case ERROR_INVALID_FILE_POINTER:
                    printf("Ошибка: указатель на файл не является валидным. \n");
                    break;
                case ERROR_OUTPUT_FILE_ERROR:
                    printf("Ошибка: не удалось записать данные в выходной файл. \n");
                    break;
                case SUCCESS:
                    succesful = true;
                    break;
            }
            break;
        case 's':
            switch (countEveryoneElse(inputFile, outputFile)) {
                case ERROR_INVALID_FILE_POINTER:
                    printf("Ошибка: указатель на файл не является валидным. \n");
                    break;
                case ERROR_OUTPUT_FILE_ERROR:
                    printf("Ошибка: не удалось записать данные в выходной файл. \n");
                    break;
                case ERROR_FILE_CORRUPTED:
                    printf("Ошибка: неопознанный символ во входном файле. \n");
                    break;
                case SUCCESS:
                    succesful = true;
                    break;
            }
            break;
        case 'a':
            switch (hexReplace(inputFile, outputFile)) {
                case ERROR_INVALID_FILE_POINTER:
                    printf("Ошибка: указатель на файл не является валидным. \n");
                    break;
                case ERROR_OUTPUT_FILE_ERROR:
                    printf("Ошибка: не удалось записать данные в выходной файл. \n");
                    break;
                case SUCCESS:
                    succesful = true;
                    break;
            }
            break;
        default:
            printf("Ошибка: проверьте написание значения флага. \n");
            break;
    }

    fclose(inputFile);
    fclose(outputFile);

    if (!succesful) {
        return 1;
    }

    printf("Программа завершена успешно. Результат записан в файл. \n");
    return 0;
}