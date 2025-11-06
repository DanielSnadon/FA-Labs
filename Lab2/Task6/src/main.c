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

    Student* students = NULL;
    int studentCount = 0;

    ErrorCode readResult = readStudents(inputFile, &students, &studentCount);
    fclose(inputFile);

    switch(readResult) {
        case SUCCESS:
            break;
        case ERROR_INVALID_DATA:
            printf("Ошибка: неверный формат данных во входном файле. \n");
            fclose(outputFile);
            return 1;
        case ERROR_MEMORY_ALLOCATION:
            printf("Ошибка: не удалось выделить необходимую память. \n");
            fclose(outputFile);
            return 1;
        case ERROR_INVALID_FILE_POINTER:
            printf("Ошибка: указатель не является валидным. \n");
            fclose(outputFile);
            return 1;
        default:
            printf("Ошибка: неизвестная ошибка. \n");
            fclose(outputFile);
            return 1;
    }

    int choice;
    int running = 1;

    while (running) {
        printMenu();

        if (scanf("%d", &choice) != 1) {
            printf("Ошибка: неверный ввод. \n");
            fclose(outputFile);
            return 1;
        }

        switch(menu(outputFile, students, studentCount, &choice)) {
            case SUCCESS:
                break;
            case SUCCESS_EXIT:
                fclose(outputFile);
                return 0;
            case ERROR_INVALID_INPUT:
                printf("Ошибка: неверный ввод. \n");
                fclose(outputFile);
                return 1;
            case ERROR_INVALID_DATA:
                printf("Ошибка: получены неверные данные. \n");
                fclose(outputFile);
                return 1;
            case ERROR_MEMORY_ALLOCATION:
                printf("Ошибка: не удалось выделить необходимую память. \n");
                fclose(outputFile);
                return 1;
            case ERROR_UNKNOWN:
                printf("Ошибка: неизвестная ошибка. \n");
                fclose(outputFile);
                return 1;
        }
    }

    printf("Ошибка: неизвестная ошибка. \n");
    fclose(outputFile);
    return 1;
}