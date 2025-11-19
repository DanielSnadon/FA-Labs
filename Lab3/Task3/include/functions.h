#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "linked_list.h"
#include <stdbool.h>

typedef enum {
    SUCCESS = 0,
    ERROR_INVALID_POINTER,
    ERROR_NO_FILE_TYPE,
    UNKNOWN_FILE_TYPE,
    ERROR_INVALID_INPUT,
    ERROR_CANT_OPEN_FILE,
    ERROR_ALLOCATION,
    ERROR_NOT_FOUND,
    ERROR_EMPTY_LIST
} ErrorCode;

LinkedList idSearch(const LinkedList *list, unsigned int id);
LinkedList firstNameSearch(const LinkedList *list, const char *name);
LinkedList secondNameSearch(const LinkedList *list, const char *name);
LinkedList thirdNameSearch(const LinkedList *list, const char *name);
LinkedList dateSearch(const LinkedList *list, Date date);
LinkedList genderSearch(const LinkedList *list, char gender);
LinkedList incomeSearch(const LinkedList *list, double income);

void printLiver(const Liver *l);

void insertLiver(LinkedList *list, LIST_TYPE l);

ErrorCode isThatFileGood(const char *name);

ErrorCode loadData(LinkedList *list, const char *fileName, History *history);

void makeHistory(History *h);

void menu(LinkedList *list, History *history);

#endif