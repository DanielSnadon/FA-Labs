#ifndef MANAGER_H
#define MANAGER_H

#include "structures.h"
#include "queue.h"
#include <stdio.h>
#include <stdbool.h>

typedef struct {
    Station *stations;
    size_t count;
    size_t capacity;
    char logFileName[256];
} MailService;

static unsigned int nextMailID = 1;

MailService* initService();

void cleanupService(MailService *service);

Station* findStation(const MailService *service, unsigned int id);

bool addConnection(MailService *service, unsigned int id1, unsigned int id2);

bool addStation(MailService *service, unsigned int id,
                unsigned int maxMails, const unsigned int *connections,
                size_t connectionsCount);

bool addMail(MailService *service, const char *type, int priority,
            unsigned int fromId, unsigned int toId, const char *data);

bool deleteStation(MailService *service, unsigned int id);

void fileMails(const MailService *service, const char *outFileName);

void consoleMails(const MailService *service);

bool markAsUndelivered(MailService *service, unsigned int id);

bool takeMail(MailService *service, unsigned int mId, unsigned int sId);

void setLogFileName(MailService *service, const char *name);

void writeLog(const MailService *service, const char *format, ...);

const char* stringStatus(MailStatus status);

#endif