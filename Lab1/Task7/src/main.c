#include "functions.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Ошибка вызова программы: неверное число аргументов. \nОжидается ввод в формате: \"./main [входной файл] [выходной файл]\". \n");
        return 1;
    }

    char *inputFileName = argv[1];

    switch(isThatFileGood(inputFileName)) {
        case ERROR_INVALID_FILE_POINTER:
            printf("Ошибка функции проверки расширения первого файла: указатель на файл не является валидным. \n");
            return 1;
        case ERROR_NO_FILE_TYPE:
            printf("Ошибка входного файла: у входного файла отсутсвует расширение. Убедитесь, что работаете с правильным файлом. \n");
            return 1;
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
            printf("Ошибка функции проверки расширения второго файла: указатель на файл не является валидным. \n");
            return 1;
        case ERROR_NO_FILE_TYPE:
            printf("Ошибка выходного файла: у выходного файла отсутсвует расширение. Убедитесь, что работаете с правильным файлом. \n");
            return 1;
        case UNKNOWN_FILE_TYPE:
            printf("Ошибка выходного файла: расширение файла не входит в число проверенных, результат может отличаться от ожидаемого. Продолжить? [y/n]: ");
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
        printf("Ошибка открытия входного файла. \n");
        return 1;
    }

    FILE *outputFile = fopen(outputFileName, "w");
    
    if (outputFile == NULL) {
        printf("Ошибка открытия выходного файла. \n");
        fclose(inputFile);
        return 1;
    }

    switch(completeTask(inputFile, outputFile)) {
        case SUCCESS:
            printf("Программа завершена успешно. Результат записан в файл. \n");
            break;
        case ERROR_INVALID_FILE_POINTER:
            printf("Ошибка функции: указатель на файл не является валидным. \n");
            break;
        case ERROR_INVALID_VALUE_IN_FILE:
            printf("Ошибка функции: обнаружен неопознанный символ во входном файле. \nОбработка чисел отменена, предыдущие значения (если имеются) вычислены успешно. \n");
            break;
        case ERROR_OVERFLOW:
            printf("Ошибка: переполнение ячейки памяти при вычислении значения числа. \nОбработка чисел отменена, предыдущие значения (если имеются) вычислены успешно. \n");
            break;
        case ERROR_OUTPUT_FILE_ERROR:
            printf("Ошибка вывода: не удалось записать значение в выходной файл. \nОбработка чисел отменена, предыдущие значения (если имеются) вычислены успешно. \n");
            break;
        case ERROR_CANT_WRITE_ERROR:
            printf("Ошибка: во время вычисления значений был обнаружен неопознанный символ или произошло переполнение ячейки памяти. \nВо время записи результата обработки ошибки в файл произошла ошибка. \nОбработка чисел отменена, предыдущие значения (если имеются) вычислены успешно. \n");
    }

    fclose(inputFile);
    fclose(outputFile);
    return 0;
}

// Одинокое отрицательное число не работает.