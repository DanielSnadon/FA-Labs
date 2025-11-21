#include "interpreter.h"
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Использование: ./main <входной файл> \n");
        return 1;
    }

    const char *inputFileName = argv[1];

    switch(isThatFileGood(inputFileName)) {
        case ERROR_INVALID_POINTER:
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

    int result;

    result = startInterpreter(inputFileName);

    if (result != 0) {
        printf("Ошибка: неверный ввод (строка: %d). \n", result);
    }

    return 0;
}