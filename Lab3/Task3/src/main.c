#include "functions.h"
#include "linked_list.h"
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {

    LinkedList villagers = create_list();

    History history;

    makeHistory(&history);

    char inFileName[256];

    printf("Введите имя файла для загрузки данных: ");

    if (fgets(inFileName, sizeof(inFileName), stdin) == NULL) {
        printf("Ошибка: неверный ввод имени файла для чтения. \n");
        return 1;
    }

    inFileName[strcspn(inFileName, "\n")] = '\0';

    switch(isThatFileGood(inFileName)) {
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

    switch (loadData(&villagers, inFileName, &history)) {
        case ERROR_INVALID_POINTER:
            printf("Ошибка функции разгрузки файла: передаваемый указатель не является валидным. \n");
            return 1;
        case ERROR_CANT_OPEN_FILE:
            printf("Ошибка функции разгрузки файла: не удалось открыть файл для чтения. \n");
            return 1;
        case ERROR_INVALID_INPUT:
            printf("Ошибка: неверный формат данных во входном файле. \n");
            return 1;
    }

    menu(&villagers, &history);

    delete_list(&villagers);

    return 0;
}