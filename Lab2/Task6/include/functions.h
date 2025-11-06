#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    SUCCESS = 0,
    SUCCESS_EXIT,
    UNKNOWN_FILE_TYPE,
    ERROR_NO_FILE_TYPE,
    ERROR_INVALID_FILE_POINTER,
    ERROR_INVALID_DATA,
    ERROR_MEMORY_ALLOCATION,
    ERROR_INVALID_INPUT,
    ERROR_UNKNOWN

} ErrorCode;

// Вспомогательные функции:

ErrorCode isThatFileGood(const char *name);

// Структура "Студент":

typedef struct {
    unsigned int id;
    char* firstName;
    char* lastName;
    char* group;
    unsigned char* grades;
} Student;

// Основные функции:

void printMenu();
ErrorCode menu(FILE *output, Student* students, const short int studentCount, const int *choice);
ErrorCode readStudents(FILE* input, Student** students, int* studCount);

#endif