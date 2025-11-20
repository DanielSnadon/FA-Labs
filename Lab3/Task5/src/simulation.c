#include "simulation.h"

// Вспомогательные функции

unsigned int searchForNextStation(const MailService *service,
    unsigned int currId, unsigned int targetId,
    unsigned int *visitedIds, size_t visitedCount)
{    
    if (currId == targetId) {
        return 1;
    }

    Station *currStation = findStation(service, currId);
    if (currStation == NULL) {
        return 0;
    }

    if (visitedCount > 50) {
        return 0;
    }

    visitedIds[visitedCount] = currId;

    unsigned int minPathLength = 0;

    for (size_t i = 0; i < currStation->connectionCount; i++) {

        unsigned int neighborId = currStation->connections[i];

        bool visited = 0;
        for (size_t j = 0; j < visitedCount; j++) {
            if (visitedIds[j] == neighborId) {
                visited = 1;
                break;
            }
        }

        if (visited) {
            continue;
        }

        unsigned int pathLength = searchForNextStation(service, neighborId, targetId, visitedIds, visitedCount + 1);

        if (pathLength > 0) {

            unsigned int currTotalLength = pathLength + 1;

            if (minPathLength == 0 || currTotalLength < minPathLength) {
                minPathLength = currTotalLength;
            }
        }
    }
    
    return minPathLength;
}

unsigned int findNextStation(const Station *currStation, unsigned int targetId, const MailService *service)
{
    unsigned int nextStationId = 0;
    
    Station *targetStation = findStation(service, targetId);

    if (targetStation != NULL && targetStation->currentMails < targetStation->maxMails) {
        
        bool connected = 0;

        for (size_t i = 0; i < currStation->connectionCount; i++) {

            if (currStation->connections[i] == targetId) {
                connected = 1;
                break;
            }
        }

        if (connected) {
            return targetId;
        }
    }

    unsigned int minPathLength = 0;

    for (size_t i = 0; i < currStation->connectionCount; i++) {

        unsigned int neighborId = currStation->connections[i];
        Station *neighbor = findStation(service, neighborId);

        if (neighbor != NULL && neighbor->currentMails < neighbor->maxMails) {

            unsigned int visited[50] = {0};
            unsigned int pathLength = searchForNextStation(service, neighborId, targetId, visited, 0);

            if (pathLength > 0) {
                if (minPathLength == 0 || pathLength < minPathLength) {
                    minPathLength = pathLength;
                    nextStationId = neighborId;
                }
            }
        }
    }

    if (nextStationId != 0) {
        return nextStationId;
    }

    for (size_t i = 0; i < currStation->connectionCount; i++) {

        unsigned int neighborId = currStation->connections[i];
        Station *neighbor = findStation(service, neighborId);

        if (neighbor != NULL && neighbor->currentMails < neighbor->maxMails) {
            return neighborId;
        }
    }

    return 0;
}

void resetStatuses(Node *currNode) {
    if (currNode == NULL) {
        return;
    }

    if (currNode->data.status == STATUS_MOVING) {
        currNode->data.status = STATUS_IN_WAY;
        currNode->data.priority = 0 - currNode->data.priority;
    }

    resetStatuses(currNode->left);
    resetStatuses(currNode->right);
}

// Основные функции

void simulateTick(MailService *service)
{
    for (size_t i = 0; i < service->count; i++) {

        Station *currStation = &service->stations[i];

        if (isQueueEmpty(currStation->mails)) {
            continue;
        }

        Mail mail;
        currStation->mails = popQueue(currStation->mails, &mail);
        currStation->currentMails--;

        if (mail.status == STATUS_MOVING) {
            mail.status = STATUS_IN_WAY;
            
            currStation->mails = addQueue(currStation->mails, mail);
            currStation->currentMails++;
            mail.priority = 0 - mail.priority;

            writeLog(service, "Письмо ID:%u (Приор: %d) получено в ID:%u в этом тике и оказалось самым приоритетным на данный момент. Никаких действий не выполнено.",
                    mail.id, mail.priority, currStation->id);

            continue;
        } else {
            resetStatuses(currStation->mails); 
        }

        if (mail.status != STATUS_IN_WAY) {
            mail.priority++;
            
            currStation->mails = addQueue(currStation->mails, mail);
            currStation->currentMails++;
            
            writeLog(service, "Письмо ID:%u (Статус: %s) проигнорировано, остаётся в пункте ID:%u.",
                    mail.id, stringStatus(mail.status), currStation->id);
            continue;
        }

        unsigned int nextStationId = findNextStation(currStation, mail.toId, service);

        if (nextStationId != 0) {

            Station *nextStation = findStation(service, nextStationId);

            mail.currentId = nextStationId;

            if (nextStationId == mail.toId) {

                writeLog(service, "Письмо ID:%u (Приор: %d) ДОСТИГЛО пункта назначения ID:%u и ожидает получения.",
                         mail.id, mail.priority, nextStationId);
                mail.priority = 0;
                mail.status = STATUS_DELIVERED;

            } else {

                writeLog(service, "Письмо ID:%u (Приор: %d) ПЕРЕСЛАНО из ID:%u в ID:%u.",
                         mail.id, mail.priority, currStation->id, nextStationId);
                mail.status = STATUS_MOVING;
                mail.priority = 0 - mail.priority;
            }

            nextStation->mails = addQueue(nextStation->mails, mail);
            nextStation->currentMails++;

        } else {
            mail.priority++;
            
            currStation->mails = addQueue(currStation->mails, mail);
            currStation->currentMails++;

            writeLog(service, "Письмо ID %u (Приор: %d) ЗАДЕРЖАНО в ID %u. Нет доступных маршрутов/соседей.",
                     mail.id, mail.priority, currStation->id);
        }
    }
}

// unused
void startSimulation(MailService *service)
{
    writeLog(service, "- - - Бесконечная симуляция запущена! - - -");

    while(1) {
        simulateTick(service);
        usleep(TICK);
    }
}

void runTicks(MailService *service, unsigned int n)
{
    writeLog(service, "- - - Симуляция на %u тиков запущена! - - -", n);

    for (unsigned int i = 0; i < n; i++) {
        writeLog(service, "- Тик №%u -", i + 1);
        simulateTick(service);
        usleep(TICK);
    }

    writeLog(service, "- - - Симуляция завершена - - -");
}