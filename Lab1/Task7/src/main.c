#include "functions.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Ошибка вызова программы: неверное число аргументов. \nОжидается ввод в формате: \"./main [входной файл] [выходной файл]\". \n");
        return 1;
    }

    char *inputFileName = argv[1];

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
    
    char *outputFileName = argv[2];

    switch(isThatFileGood(outputFileName)) {
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
    
    char* shrtInptFlNm = strrchr(inputFileName, '/');
    char* shrtOutptFlNm = strrchr(outputFileName, '/');

    shrtInptFlNm = (shrtInptFlNm != NULL) ? shrtInptFlNm + 1 : inputFileName;
    shrtOutptFlNm = (shrtOutptFlNm != NULL) ? shrtOutptFlNm + 1 : outputFileName;


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

    switch(func(inputFile, outputFile)) {
        case SUCCESS:
            printf("WOOHOO!");
            return 1;
        case ERROR_INVALID_FILE_POINTER:
            printf("Ошибка функции: указатель на файл не является валидным. \n");
            return 0;
        case ERROR_INVALID_VALUE_IN_FILE:
            printf("Ошибка функции: обнаружен неопознанный символ во входном файле. \n");
            return 0;
    }

    return 0;
}