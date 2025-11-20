#ifndef MENU_H
#define MENU_H

#include "simulation.h"
#include <string.h>

typedef enum {
    SUCCESS = 0,
    ERROR_INVALID_POINTER,
    ERROR_NO_FILE_TYPE,
    UNKNOWN_FILE_TYPE
} ErrorCode;

ErrorCode isThatFileGood(const char *name);

void menu(MailService *service);

// unused
bool readStations(MailService *service, const char *fileName);

int getInt();

MailService *initializeSystem(const char *fileName);

#endif