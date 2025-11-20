#include "menu.h"

ErrorCode isThatFileGood(const char *name)
{
    if (name == NULL) {
        return ERROR_INVALID_POINTER;
    }
    const char *fileTypes[] = {".txt", ".docx", "doc", "rtf", ".c", ".h", ".cpp", ".hpp", ".java", ".py", ".csv", ".json", ".xml", ".html", ".css", ".js", ".md", ".log", ".conf", ".config", NULL};

    char *splitter = strrchr(name, '.');

    if (splitter == NULL) {
        return ERROR_NO_FILE_TYPE;
    }

    for (int i = 0; fileTypes[i] != NULL; i++) {
        if (strcmp(splitter, fileTypes[i]) == 0) {
            return SUCCESS;
        }
    }

    return UNKNOWN_FILE_TYPE;
}

int getInt()
{
    int value;
    char buffer[100];

    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }
    if (sscanf(buffer, "%d", &value) != 1) {
        return -1;
    }

    return value;
}

// unused
bool readStations(MailService *service, const char *fileName)
{
    if (fileName == NULL || service == NULL) {
        return 0;
    }
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Ошибка: не удалось открыть файл для чтения. \n");
        return 0;
    }

    char line[512];
    int lineCount = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        lineCount++;

        line[strcspn(line, "\n")] = '\0';

        char *token = strtok(line, ",");
        if (token == NULL) {
            continue;
        }

        unsigned int id, maxMails, connectionsCount;

        if (sscanf(token, "%u", &id) != 1) {
            printf("Ошибка: неверный формат данных в файле. \n");
            return 0;
        }

        token = strtok(NULL, ",");
        if (token == NULL || sscanf(token, "%u", &maxMails) != 1) {
            printf("Ошибка: неверный формат данных в файле. \n");
            return 0;
        }

        token = strtok(NULL, ",");
        if (token == NULL || sscanf(token, "%u", &connectionsCount) != 1) {
            printf("Ошибка: неверный формат данных в файле. \n");
            return 0;
        }

        unsigned int *connections = (unsigned int*)malloc(connectionsCount * sizeof(unsigned int));
        if (connections == NULL) {
            printf("Ошибка: не удалось выделить необходимую память. \n");
            return 0;
        }

        for (unsigned int i = 0; i < connectionsCount; i++) {
            token = strtok(NULL, ",");
            
            if (token == NULL || sscanf(token, "%u", connections[i]) != 1) {
                printf("Ошибка: неверный формат данных в файле. \n");
                free(connections);
                return 0;
            }
        }

        if (!addStation(service, id, maxMails, connections, connectionsCount)) {
            printf("Ошибка: не удалось создать отделение. \n");
            free(connections);
            return 0;
        }

        free(connections);
    }

    if (lineCount == 0) {
        printf("Ошибка: файл для чтения пуст. \n");
    }

    fclose(file);
    
    return 1;
}

MailService *initializeSystem(const char *fileName)
{
    if (fileName == NULL) {
        printf("Ошибка: неверно указано имя файла. \n");
        return NULL;
    }

    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Ошибка: не удалось открыть файл. \n");
        return NULL;
    }

    MailService *service = initService();
    if (service == NULL) {
        fclose(file);
        return NULL;
    }

    setLogFileName(service, NULL);

    char line[512];
    unsigned int idA, idB;
    int successes = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        
        if (sscanf(line, "%u, %u", &idA, &idB) == 2) {

            if (findStation(service, idA) == NULL) {

                if (addStation(service, idA, 10, NULL, 0)) {
                    writeLog(service, "Создана новая станция ID:%u (Вместимость: 10).", idA);
                }
            }

            if (findStation(service, idB) == NULL) {

                if (addStation(service, idB, 10, NULL, 0)) {
                    writeLog(service, "Создана новая станция ID:%u (Вместимость: 10).", idB);
                }
            }

            if (addConnection(service, idA, idB)) {
                successes++;
            }

        } else {
            printf("Ошибка: неверный формат данных входного файла. \n");
            cleanupService(service);
            fclose(file);
        }
    }

    fclose(file);
    writeLog(service, "Загрузка завершена. Успешно создано %d двухсторонних связей.", successes);
    
    return service;
}

void menu(MailService *service)
{
    if (service == NULL) {
        printf("Ошибка: основная структура программы не сгенерирована. \n");
        return;
    }

    int choice;
    int id, maxMails, connectionsCount, ticks;
    unsigned int mailId, stationId;

    do {
        printf("\n\n- - - Главное меню - - - \n");
        printf("1. Добавить отделение \n");
        printf("2. Удалить отделение \n");
        printf("3. Добавить письмо \n");
        printf("4. Имитировать работу (N тиков) \n");
        printf("5. Вывести все письма (консоль) \n");
        printf("6. Сохранить все письма (файл) \n");
        printf("7. Пометить письмо как недоставленное \n");
        printf("8. Попытка взять письмо \n");
        printf("0. Выход \n");
        printf("Ваш выбор: "); 

        choice = getInt();

        if (choice < 0 || choice > 8) {
            printf("Ошибка: некорректный ввод. Попробуйте ещё раз. \n");
            continue;
        }

        switch (choice) {
            case 1:
                printf("Введите ID: ");
                id = getInt();
                if (id < 0) {
                    printf("Ошибка: неверный ввод. \n");
                    break;
                }

                printf("Введите максимальную вместимость: ");
                maxMails = getInt();
                if (maxMails < 0) {
                    printf("Ошибка: неверный ввод. \n");
                    break;
                }

                printf("Введите количество соседних отделений: ");
                connectionsCount = getInt();
                if (connectionsCount < 0) {
                    printf("Ошибка: неверный ввод. \n");
                    break;
                }

                unsigned int *connections = (unsigned int*)malloc(connectionsCount * sizeof(unsigned int));
                if (connections == NULL) {
                    printf("Ошибка: не удалось выделить необходимую память. \n");
                    return;
                }

                printf("Вводите ID соседних отделений нового отделения (через Enter): \n");
                for (int i = 0; i < connectionsCount; i++) {
                    
                    int connectionId = getInt();
                    connections[i] = (unsigned int)connectionId;
                }

                addStation(service, (unsigned int)id, (unsigned int)maxMails, connections, (unsigned int)connectionsCount);
                free(connections);

                break;

            case 2:
                printf("Введите ID отделения для удаления: ");
                id = getInt();
                if (id < 0) {
                    break;
                }

                deleteStation(service, (unsigned int)id);

                break;

            case 3:
                char type[64], data[128];

                printf("Введите приоритет письма: ");
                int priority = getInt();
                if (priority < 0) {
                    printf("Ошибка: неверный ввод. \n");
                    break;
                }

                printf("Введите ID отделения отправителя письма: ");
                unsigned int fromId = getInt();

                printf("Введите ID отделения получателя письма: ");
                unsigned int toId = getInt();

                printf("Введите тип письма: ");
                if (fgets(type, sizeof(type), stdin) == NULL) {
                    printf("Ошибка: неверный ввод. \n");
                    break;
                }
                type[strcspn(type, "\n")] = '\0';

                printf("Введите технические данные письма: ");
                if (fgets(data, sizeof(data), stdin) == NULL) {
                    printf("Ошибка: неверный ввод. \n");
                    break;
                }
                data[strcspn(data, "\n")] = '\0';

                addMail(service, type, priority, fromId, toId, data);

                break;

            case 4:
                printf("Введите количество тиков: ");
                ticks = getInt();
                if (ticks <= 0) {
                    printf("Ошибка: неверный ввод. \n");
                    break;
                }

                runTicks(service, (unsigned int)ticks);
                break;

            case 5:
                consoleMails(service);
                break;

            case 6:
                char outFileName[256];
                printf("Введите имя файла для сохранения: ");

                if (fgets (outFileName, sizeof(outFileName), stdin) == NULL) {
                    printf("Ошибка: неверный ввод. \n");
                    break;
                }

                outFileName[strcspn(outFileName, "\n")] = '\0';

                bool anyErrors = 0;

                switch(isThatFileGood(outFileName)) {
                    case ERROR_INVALID_POINTER:
                        printf("Ошибка функции проверки расширения первого файла: указатель на файл не является валидным. \n");
                        anyErrors = 1;
                        break;
                    case ERROR_NO_FILE_TYPE:
                        printf("Ошибка входного файла: у входного файла отсутсвует расширение. Убедитесь, что работаете с правильным файлом. \n");
                        anyErrors = 1;
                        break;
                    case UNKNOWN_FILE_TYPE:
                        printf("Ошибка входного файла: расширение файла не входит в число проверенных, результат может отличаться от ожидаемого. Продолжить? [y/n]: ");
                        if (getchar() != 'y') {
                            anyErrors = 1;
                            break;
                        }
                        break;
                    }

                if (!anyErrors) {
                    fileMails(service, outFileName);
                }
                
                break;

            case 7:
                printf("Введите ID письма: ");
                mailId = getInt();
                markAsUndelivered(service, mailId);
                break;

            case 8:
                printf("Введите ID письма: ");
                mailId = getInt();
                printf("Введите ID отделения, откуда попробовать забрать: ");
                stationId = getInt();
                takeMail(service, mailId, stationId);
                break;

            case 0:
                printf("Работа завершена. \n");
                break;
        }

    } while (choice != 0);
}