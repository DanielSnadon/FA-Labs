#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <stdio.h>

#define VAR_COUNT 26
#define FILE_NAME "out.txt"
#define MAX_LENGTH 512

typedef struct {
    long long variables[VAR_COUNT];
    int isInited[VAR_COUNT];
    FILE* traceFile;
} appState;

void createInterpreter(appState* state);

#endif