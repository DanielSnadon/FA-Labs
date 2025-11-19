#ifndef MANAGER_H
#define MANAGER_H

#include "structures.h"
#include <stdio.h>
#include <stdbool.h>

typedef struct {
    Station *stations;
    size_t count;
    size_t capacity;
} MailService;

static unsigned int nextMailID = 1;

MailService* initService();

void cleanupService(MailService *service);

bool addStation(MailService *service, unsigned int id,
                unsigned int maxMails, const unsigned int *connections,
                size_t connectionsCount);

bool addMail(MailService *service, const char *type, int priority,
            unsigned int fromId, unsigned int toId, const char *data);

#endif