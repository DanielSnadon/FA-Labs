#include "interpreter.h"

ErrorCode isThatFileGood(const char *name)
{
    if (name == NULL) {
        return ERROR_INVALID_POINTER;
    }
    const char *fileTypes[] = {".txt", ".docx", "doc", "rtf", ".c", ".h", ".cpp", ".hpp", ".java", ".py", ".csv", ".json", ".xml", ".html", ".css", ".js", ".md", ".toLog", ".conf", ".config", NULL};

    char *splitter = strrchr(name, '.');

    if (splitter == NULL) {
        return ERROR_NO_FILE_TYPE;
    }

    for (int i = 0; fileTypes[i] != NULL; i++) {
        if (strcmp(splitter, fileTypes[i]) == 0) {
            return SUCCESS;
        }
    }

    return UNKNOWN_FILE_TYPE;
}

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

void toLog(appState* state, int lineNumber, const char* command, const char* description)
{
    if (state->traceFile == NULL || state == NULL || command == NULL || description == NULL) {
        return;
    }

    char varBuffer[MAX_LENGTH];

    stringVars(state, varBuffer, sizeof(varBuffer));

    fprintf(state->traceFile, "[%d] %s |", lineNumber, command);
    fprintf(state->traceFile, " %s | %s\n", varBuffer, description);

    fflush(state->traceFile);
}

void createInterpreter(appState* state)
{
    for (int i = 0; i < VAR_COUNT; i++) {
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

    for (long long t = 0; t < n; t++) {
        res *= b;
    }

    return res;
}

int getPriority(char op)
{
    if (op == '^') {
        return 3;
    }
    if (op == '*' || op == '/') {
        return 2;
    }
    if (op == '+' || op == '-') {
        return 1;
    }

    return 0;   
}

int completeOperation(char op, long long* operands, int* top)
{
    if (*top < 1) {
        return -1;
    }

    long long b = operands[(*top)--];
    long long a = operands[(*top)--];

    long long result;

    switch (op) {
        case '+':
            result = a + b;
            break;
        case '-':
            result = a - b;
            break;
        case '*':
            result = a * b;
            break;
        case '/':
            if (b == 0) {
                return -1;
            }
            result = a / b;
            break;
        case '^':
            result = power(a, b);
            break;
        default:
            return -1;
    }

    operands[++(*top)] = result;

    return 0;
}

int solveLine(appState* state, const char* line, long long* result)
{
    long long operands[STACK_SIZE];
    int andTop = -1;

    char operators[STACK_SIZE];
    int torTop = -1;

    const char* curr = line;
    char* last;

    while (*curr) {
        if (isspace(*curr)) {
            curr++;
            continue;
        }

        if (isdigit(*curr)) {

            long long number = strtoll(curr, &last, 10);

            if (andTop >= STACK_SIZE - 1) {
                return -1;
            }

            operands[++andTop] = number;
            curr = last;

            continue;

        } else if (isupper(*curr)) {

            int varIndex = *curr - 'A';

            if (!state->isInited[varIndex]) {
                return -1;
            }

            if (andTop >= STACK_SIZE - 1) {
                return -1;
            }

            operands[++andTop] = state->variables[varIndex];
            curr++;
            continue;
        }

        char currOperator = *curr;

        if (currOperator == '+' || currOperator =='-' || currOperator == '*'
            || currOperator == '/' || currOperator =='^') {

                char topOperator;
                if (torTop >= 0) {
                    topOperator = operators[torTop];
                } else {
                    topOperator = '\0';
                }

                while (torTop != -1 && getPriority(topOperator) >= getPriority(currOperator)) {

                    char workingOperator = operators[torTop--];

                    if (completeOperation(workingOperator, operands, &andTop) != 0) {
                        return -1;
                    }

                    if (torTop >= 0) {
                        topOperator = operators[torTop];
                    } else {
                        topOperator = '\0';
                    }
                    
                }

                if (torTop >= STACK_SIZE - 1) {
                    return -1;
                }

                operators[++torTop] = currOperator;

                curr++;
                continue;
        }

        return -1;
    }

    while (torTop != -1) {

        char currOp = operators[torTop--];
        
        if (completeOperation(currOp, operands, &andTop) != 0) {
            return -1;
        }
    }

    if (andTop != 0) {
        return -1;
    }

    *result = operands[andTop--];

    return 0;
}

int translateLine(appState* state, const char* line, int lineNumber)
{
    char fixedLine[MAX_LENGTH];
    strncpy(fixedLine, line, MAX_LENGTH -1);
    fixedLine[MAX_LENGTH - 1] = '\0';

    char* curr = fixedLine;

    while (isspace(*curr)) {
        curr++;
    }

    size_t len = strlen(curr);

    while (len > 0 && isspace(curr[len - 1])) {
        len--;
    }

    curr[len] = '\0';

    if (len == 0) {
        return 0;
    }

    if (strncmp(curr, "print(", 6) == 0 && curr[len - 1] == ')') {

        char* expressionStart = curr + 6;
        char expression[MAX_LENGTH];

        strncpy(expression, expressionStart, len - 7);

        expression[len - 7] = '\0';
        
        if (len - 7 == 0) {
            return -1;
        }

        long long result;

        if (solveLine(state, expression, &result) != 0) {
            return -1;
        }

        fprintf(stdout, "%lld\n", result);

        toLog(state, lineNumber, line, "Print()");

        return 0;
    }

    if (len >= 3 && isupper(curr[0])) {

        int varIndex = curr[0] - 'A';
        char* ravno = strchr(curr, '=');

        if (ravno != NULL) {
            
            char* expressionStart = ravno + 1;

            while (isspace(*expressionStart)) {
                expressionStart++;
            }

            long long result;

            if (solveLine(state, expressionStart, &result) != 0) {
                return -1;
            }

            state->variables[varIndex] = result;
            state->isInited[varIndex] = 1;

            toLog(state, lineNumber, line, "Assignment / Arithmetic operation");
            return 0;
        }
    }

    return -1;
}

int startInterpreter(const char* inFileName) {
    FILE *in = fopen(inFileName, "r");
    if (in == NULL) {
        return -1;
    }

    appState state;
    createInterpreter(&state);

    if (state.traceFile == NULL) {
        fclose(in);
        return -1;
    }

    char line[MAX_LENGTH];
    int lineNumber = 0;
    int success = 0;

    while (fgets(line, sizeof(line), in)) {
        lineNumber++;
        line[strcspn(line, "\n")] = 0;

        success = translateLine(&state, line, lineNumber);
        if (success != 0) {
            break;
        }
    }

    fclose(in);
    clean(&state);

    if (success == 0) {
        return 0;
    } else {
        return lineNumber;
    }
}