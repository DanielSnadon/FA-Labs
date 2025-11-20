#include "manager.h"
#include <time.h>
#include <string.h>
#include <stdarg.h>

// Вспомогательные функции

unsigned int generateMailID()
{
    return nextMailID++;
}

int findStationIndex(const MailService *service, unsigned int id)
{
    for (size_t i = 0; i < service->count; i++) {

        if (service->stations[i].id == id) {

            return (int)i;
        }
    }

    return -1;
}

Station* findStation(const MailService *service, unsigned int id)
{
    int index = findStationIndex(service, id);

    if (index != -1) {
        return &service->stations[index];
    }

    return NULL;
}

void throwMailsAway(MailService *service, Station *station)
{
    Mail currMail;

    while (!isQueueEmpty(station->mails)) {

        station->mails = popQueue(station->mails, &currMail);
        station->currentMails--;

        if (currMail.fromId == station->id || currMail.toId == station->id) {
            currMail.status = STATUS_UNDELIVERED;

            writeLog(service, "Письмо ID:%u помечено как НЕДОСТАВЛЕННОЕ и удалено (Причина: удаление отделения ID:%u).",
                    currMail.id, station->id);

        } else {

            if (station->connectionCount > 0) {
                
                bool foundHome = 0;

                for (size_t i = 0; i < station->connectionCount; i++) {

                    unsigned int nextStationId = station->connections[i];
                    Station *nextStation = findStation(service, nextStationId);

                    if (nextStation != NULL && nextStation->currentMails < nextStation->maxMails) {
                        
                        currMail.currentId = nextStationId;
                        nextStation->mails = addQueue(nextStation->mails, currMail);
                        nextStation->currentMails++;

                        foundHome = 1;

                        writeLog(service, "Письмо ID:%u перенаправлено из ID %u в ID %u (Причина: удаление отделения ID:%u).",
                                currMail.id, station->id, nextStationId, station->id);

                        break;
                    }
                }

                if (!foundHome) {
                    currMail.status = STATUS_UNDELIVERED;
                    writeLog(service, "Письмо ID:%u не нашло доступных отделений. Письмо помечено как НЕДОСТАВЛЕННОЕ и удалено (Причина: удаление отделения ID:%u).",
                            currMail.id, station->id);
                }
            
            } else {
                currMail.status = STATUS_UNDELIVERED;
                writeLog(service, "Письмо ID:%u не нашло доступных отделений. Письмо помечено как НЕДОСТАВЛЕННОЕ и удалено (Причина: удаление отделения ID:%u).",
                            currMail.id, station->id);
            }
        }
    }
}

Node* findMail(Node *r, unsigned int id, Mail *mail)
{
    if (r == NULL || mail == NULL) {
        return NULL;
    }

    if (r->data.id == id) {
        *mail = r->data;
        return r;
    }

    Node *result = findMail(r->left, id, mail);
    if (result != NULL) {
        return result;
    }

    return findMail(r->right, id, mail);
}

Station* findMailStation(MailService *service, unsigned int id, Mail *mail, Node **mailNode)
{
    *mailNode = NULL;

    for (size_t i = 0; i < service->count; i++) {
        Station *currStation = &service->stations[i];

        Node *found = findMail(currStation->mails, id, mail);

        if (found != NULL) {
            *mailNode = found;
            return currStation;
        }
    }

    return NULL;
}

const char* stringStatus(MailStatus status)
{
    switch (status) {
        case STATUS_IN_WAY: return "В ПУТИ";
        case STATUS_DELIVERED: return "ДОСТАВЛЕНО";
        case STATUS_UNDELIVERED: return "НЕ ДОСТАВЛЕНО";
        default: return "НЕИЗВЕСТНО";
    }
}

void printMail(const Mail *mail, FILE *stream)
{
    if (mail == NULL || stream == NULL) {
        return;
    }

    fprintf(stream, "| ID: %-7u | Приоритет: %-3d | Статус: %s | Отпр: %-5u | Получ: %-5u | Текущее: %-5u | Тип: %-10s | Тех. данные: %s \n",
            mail->id, mail->priority, stringStatus(mail->status),
            mail->fromId, mail->toId, mail->currentId, mail->type,
            mail->data);
}

void printMails(Node *r, FILE *output)
{
    if (output == NULL || r == NULL) {
        return;
    }

    printMail(&(r->data), output);

    printMails(r->left, output);
    printMails(r->right, output);
}

bool connect(Station *targetStation, unsigned int connectionId)
{
    if (targetStation == NULL) {
        return 0;
    }

    for (size_t i = 0; i < targetStation->connectionCount; i++) {

        if (targetStation->connections[i] == connectionId) {
            return 1;
        }
    }

    unsigned int newCount = targetStation->connectionCount + 1;

    unsigned int *newConnections = (unsigned int*)realloc(targetStation->connections, newCount * sizeof(unsigned int));
    if (newConnections == NULL) {
        return 0;
    }

    targetStation->connections = newConnections;
    targetStation->connections[targetStation->connectionCount] = connectionId;
    targetStation->connectionCount = newCount;

    return 1;
}

// Основные функции

bool addConnection(MailService *service, unsigned int id1, unsigned int id2) {
    if (service == NULL || id1 == id2) {
        return 0;
    }

    Station *s1 = findStation(service, id1);
    Station *s2 = findStation(service, id2);

    if (s1 == NULL || s2 == NULL) {
        return 0;
    }

    bool status1 = connect(s1, id2);
    bool status2 = connect(s2, id1);

    if (!status1 || !status2) {
        return 0;
    }

    return 1;
}

bool addStation(MailService *service, unsigned int id,
                unsigned int maxMails, const unsigned int *connections,
                size_t connectionsCount)
{
    
    if (findStation(service, id) != NULL) {
        printf("Ошибка: отделение с подобным ID уже существует. \n");
        return 0;
    }

    if (service->count == service->capacity) {

        size_t newCapacity;
        if (service->capacity == 0) {
            newCapacity = 4;
        } else {
            newCapacity = service->capacity * 2;
        }

        Station *newStations = (Station*)realloc(service->stations, newCapacity * sizeof(Station));

        if (newStations == NULL) {
            printf("Ошибка: не удалось выделить необходимую память. \n");
            return 0;
        }

        service->stations = newStations;
        service->capacity = newCapacity;
    }

    Station *newStation = &service->stations[service->count];
    newStation->id = id;
    newStation->maxMails = maxMails;
    newStation->currentMails = 0;
    newStation->mails = NULL;

    newStation->connections = (unsigned int*)malloc(connectionsCount * sizeof(unsigned int));
    if (newStation->connections == NULL) {
        printf("Ошибка: не удалось выделить необходимую память. \n");
        return 0;
    }

    for (size_t i = 0; i < connectionsCount; i++) {
        newStation->connections[i] = connections[i];
    }
    newStation->connectionCount = connectionsCount;

    service->count++;

    writeLog(service, "Отделение ID:%u успешно добавлено. Вместимость: %u.", id, maxMails);

    return 1;
}

bool addMail(MailService *service, const char *type, int priority,
            unsigned int fromId, unsigned int toId, const char *data)
{
    Station *sender = findStation(service, fromId);
    if (sender == NULL) {
        printf("Ошибка: указанного отделения отправителя не существует. \n");
        return 0;
    }

    if (findStation(service, toId) == NULL) {
        printf("Предупреждение: указанного отделения получателя не существует. Письмо может быть не доставлено. \n");
    }

    if (sender->currentMails >= sender->maxMails) {
        printf("Ошибка: отделение отправителя переполнено. \n");
        return 0;
    }

    Mail newMail;
    newMail.id = generateMailID();
    strncpy(newMail.type, type, sizeof(newMail.type) - 1);
    newMail.type[sizeof(newMail.type) - 1] = '\0';
    newMail.status = STATUS_IN_WAY;
    newMail.priority = priority;
    newMail.fromId = fromId;
    newMail.toId = toId;
    newMail.currentId = fromId;
    strncpy(newMail.data, data, sizeof(newMail.data) - 1);
    newMail.data[sizeof(newMail.data) - 1] = '\0';

    sender->mails = addQueue(sender->mails, newMail);
    sender->currentMails++;

    writeLog(service, "Письмо ID:%u (Приоритет: %d) успешно добавлено в отделение ID:%u.",
            newMail.id, newMail.priority, fromId);
    
    return 1;
}


bool deleteStation(MailService *service, unsigned int id)
{
    int targetIndex = findStationIndex(service, id);

    if (targetIndex == -1) {
        printf("Ошибка: отделение ID:%u не найдено. \n", id);
        return 0;
    }

    Station *removed = &service->stations[targetIndex];

    writeLog(service, "Удаление отделения ID:%u...", id);

    throwMailsAway(service, removed);

    free(removed->connections);

    removed->mails = NULL;

    if (targetIndex != service->count - 1) {
        service->stations[targetIndex] = service->stations[service->count - 1];
    }

    service->count--;

    writeLog(service, "Отделение ID:%u удалено успешно!", id);

    return 1;
}

void fileMails(const MailService *service, const char *outFileName)
{
    FILE *output = fopen(outFileName, "w");
    if (output == NULL) {
        printf("Ошибка: не удалось открыть файл для записи. \n");
        return;
    }

    fprintf(output, "- - - Список всех писем - - - \n");
    fprintf(output, "| ID      | Приоритет | Статус         | Отпр  | Получ | Текущее | Тип        | Данные \n");

    for (size_t i = 0; i < service->count; i++) {
        Station *currStation = &service->stations[i];

        fprintf(output, "\n- - Отделение ID:%u (Писем: %u / %u) - - \n",
                currStation->id, currStation->currentMails, currStation->maxMails);
        
        printMails(currStation->mails, output);
    }

    fclose(output);

    printf("Список всех писем был сохранён в файл: \"%s\". \n", outFileName);
}

void consoleMails(const MailService *service)
{
    printf("\n- - - Список всех писем - - - \n");
    printf("|      ID     |    Приоритет   |   Статус   | Отпр  | Получ  | Текущее | Тип | Данные \n");

    for (size_t i = 0; i < service->count; i++) {
        Station *currStation = &service->stations[i];

        printf("\n- - Отделение ID:%u (Писем: %u / %u) - - \n",
                currStation->id, currStation->currentMails, currStation->maxMails);
        
        printMails(currStation->mails, stdout);
    }
    
    printf("Список всех писем был отображён в консоль. \n");
}

bool markAsUndelivered(MailService *service, unsigned int id)
{
    Mail found;
    Node *mailNode = NULL;
    Station *station = findMailStation(service, id, &found, &mailNode);

    if (station == NULL) {
        printf("Ошибка: письма ID:%u нет ни в одном отделении. \n", id);
        return 0;
    }

    found.status = STATUS_UNDELIVERED;

    Node *newMailList = NULL;
    Mail temp;

    while (!isQueueEmpty(station->mails)) {
        station->mails = popQueue(station->mails, &temp);

        if (temp.id != id) {
            newMailList = addQueue(newMailList, temp);
        } else {
            station->currentMails--;
        }
    }

    station->mails = newMailList;

    writeLog(service, "Письмо ID:%u помечено как НЕДОСТАВЛЕНО и удалено.", id);

    return 1;
}

bool takeMail(MailService *service, unsigned int mId, unsigned int sId)
{
    Mail found;
    Node *mailNode = NULL;
    Station *station = findMailStation(service, mId, &found, &mailNode);

    if (station == NULL || station->id != sId) {
        printf("Не удалось взять письмо: письмо ID:%u не найдено в отделении ID:%u. \n", mId, sId);
        return 0;
    }

    if (found.toId != sId) {
        printf("Не удалось взять письмо: письмо ID:%u присутствует в данном отделении, но отделение не является местом назначения. \n", mId);
        return 0;
    }

    found.status = STATUS_DELIVERED;
    printf("Письмо ID:%u успешно доставлено в отделение ID:%u и передано получателю. \n", mId, sId);

    Node *newMailList = NULL;
    Mail temp;

    while (!isQueueEmpty(station->mails)) {
        station->mails = popQueue(station->mails, &temp);

        if (temp.id != mId) {
            newMailList = addQueue(newMailList, temp);
        } else {
            station->currentMails--;
        }
    }

    station->mails = newMailList;

    return 1;
}

MailService* initService()
{
    MailService *service = (MailService*)malloc(sizeof(MailService));
    
    if (service == NULL) {
        printf("Ошибка: не удалось выделить необходимую память. \n");
        return NULL;
    }

    service->stations = NULL;
    service->count = 0;
    service->capacity = 0;

    return service;
}

void cleanupService(MailService *service)
{
    if (service == NULL) {
        return;
    }

    for (size_t i = 0; i < service->count; i++) {

        freeQueue(service->stations[i].mails);
        free(service->stations[i].connections);
    }

    free(service->stations);

    free(service);
}

void setLogFileName(MailService *service, const char *name)
{
    if (service == NULL) {
        return;
    }

    if (name == NULL) {
        srand(time(NULL));
        
        unsigned long randomId = rand() % 99999 + 10000;

        snprintf(service->logFileName, sizeof(service->logFileName), "LOG_%lu.txt", randomId);
    
    } else {

        strncpy(service->logFileName, name, sizeof(service->logFileName) - 1);
        service->logFileName[sizeof(service->logFileName) - 1] = '\0';
    }

    FILE *f = fopen(service->logFileName, "w");
    if (f != NULL) {
        fprintf(f, "Начало логирования... \n");
        fclose(f);
    }
}

void writeLog(const MailService *service, const char *format, ...)
{
    char buffer[512];
    va_list args;

    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    printf("%s\n", buffer);

    if (strlen(service->logFileName) > 0) {

        FILE *f = fopen(service->logFileName, "a");

        if (f != NULL) {
            fprintf(f, "%s\n", buffer);
            fclose(f);

        } else {
            fprintf(stderr, "Внимание: не удалось открыть файл лога: %s. \n", service->logFileName);
        }
    }
}
