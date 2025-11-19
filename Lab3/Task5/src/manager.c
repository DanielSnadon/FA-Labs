#include "manager.h"
#include "queue.h"
#include <string.h>

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

// Основные функции

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

    for (unsigned int i = 0; i < connectionsCount; i++) {
        newStation->connections = connections[i];
    }
    newStation->connectionCount = connectionsCount;

    service->count++;

    printf("Отделение ID:%u успешно добавлено. Вместимость: %u. \n", id, maxMails);

    return 1;
}

bool addMail(MailService *service, const char *type, int priority,
            unsigned int fromId, unsigned int toId, const char *data)
{
    Station *sender = findStation(service, fromId);
    if (sender == NULL) {
        printf("Ошибка: указанного отделения отправителя не существует. \n");
        return false;
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

    printf("Письмо ID:%u (Приоритет: %d) успешно добавлено в отделение ID:%u. \n",
            newMail.id, newMail.priority, fromId);
    
    return 1;
}

// ---------------- CURRENT PROGRESS ----------------

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