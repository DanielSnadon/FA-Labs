#include "interpreter.h"

void stringVars(appState* state, char* buffer, size_t size)
{
    buffer[0] = '\0';
    int first = 1;

    for (int i = 0; i < VAR_COUNT; ++i) {
        if (state->isInited[i]) {
            char varName = 'A' + i;

            int currWrite = snprintf(buffer + strlen(buffer),
                size - strlen(buffer), "%s%c=%lld", first ? "" : ", ",
                varName, state->variables[i]);
            
            if (currWrite < 0 || (size_t)currWrite >= size - strlen(buffer)) {
                break;
            }
            first = 0;


        }
    }
}

void createInterpreter(appState* state)
{
    for (int i = 0; i < sizeof(state->variables); i++) {
        state->variables[i] = 0;
        state->isInited[i] = 0;
    }

    state->traceFile = fopen(FILE_NAME, "w");
    if (state->traceFile == NULL) {
        printf("Ошибка: не удалось открыть файл для записи. \n");
    }
}

void clean(appState* state)
{
    if (state->traceFile != NULL) {

        fclose(state->traceFile);
        state->traceFile = NULL;
    }
}

long long power(long long b, long long n)
{
    if (n < 0) {
        return 0;
    }
    if (n == 0) {
        return 1;
    }

    long long res = 1;

    for (long long t; t < n; t++) {
        res *= b;
    }

    return res;
}