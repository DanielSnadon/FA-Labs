#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define VAR_COUNT 26
#define FILE_NAME "out.txt"
#define MAX_LENGTH 512
#define STACK_SIZE 128

typedef enum {
    SUCCESS = 0,
    ERROR_INVALID_POINTER,
    ERROR_NO_FILE_TYPE,
    UNKNOWN_FILE_TYPE
} ErrorCode;

ErrorCode isThatFileGood(const char *name);

typedef struct {
    long long variables[VAR_COUNT];
    int isInited[VAR_COUNT];
    FILE* traceFile;
} appState;


int startInterpreter(const char* inFileName);

#endif