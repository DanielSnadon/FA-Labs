#include "functions.h"

int main(int argc, char* argv[]) {
    if (argc != 1) {
        printf("Ошибка вызова программы: неверное число аргументов. \nОжидается ввод в формате: \"./main\". \n");
        return 1;
    }

    char inputFileName[256];
    char outputFileName[256];

    printf("Введите имя входного файла: ");
    if (fgets(inputFileName, sizeof(inputFileName), stdin) == NULL) {
        printf("Ошибка входного файла: неверный ввод. \n");
        return 1;
    }
    inputFileName[strcspn(inputFileName, "\n")] = '\0';

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

    printf("Введите имя выходного файла: ");
    if (fgets(outputFileName, sizeof(outputFileName), stdin) == NULL) {
        printf("Ошибка выходного файла: неверный ввод. \n");
        return 1;
    }
    outputFileName[strcspn(outputFileName, "\n")] = '\0';

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
            fclose(inputFile);
            fclose(outputFile);
            printf("Программа завершена успешно. Результат записан в файл. \n");
            return 0;
        case ERROR_MEMORY_ALLOCATION:
            printf("Ошибка выделения памяти. \n");
            break;
        case ERROR_INVALID_OUTPUT:
            printf("Ошибка записи в выходной файл. \n");
            break;
    }

    fclose(inputFile);
    fclose(outputFile);
    return 1;
}