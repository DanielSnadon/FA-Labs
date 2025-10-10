#include "functions.h"

#include <stdbool.h>
#include <ctype.h>
#include <math.h>

ErrorCode isThatFileGood(const char *name)
{
    if (name == NULL) {
        return ERROR_INVALID_FILE_POINTER;
    }
    const char *fileTypes[] = {".txt", ".docx", "doc", "rtf", ".c", ".h", ".cpp", ".hpp", ".java", ".py", ".csv", ".json", ".xml", ".html", ".css", ".js", ".md", ".log", ".conf", ".config", NULL};

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

ErrorCode translateNumber (const char* buff, const short int currCharPosition,
    const short int sign, const short int base, double *value, FILE *output)
    {
        double answer = 0;
        for (int i = 0; i < currCharPosition; i++) {            
            if (buff[i] == '-') {
                continue;
            }

            int digit = (isdigit(buff[i]) ? buff[i] -'0' : buff[i] -'A'+ 10);

            if (answer > ((__DBL_MAX__ - digit) / base) || answer < 0 || isinf(answer) || isnan(answer)) {
                if (fprintf(output, "\nОснование системы счисления: %d\nОшибка: [Переполнение ячейки памяти]", base) <= 0) {
                    return ERROR_CANT_WRITE_ERROR;
                }
                return ERROR_OVERFLOW;
            }

            answer = answer * base + digit;
        }

        *value = answer * sign;
        
        return SUCCESS;
    }

short int symbolType(char c) {
    if ((c >= 'A' && c <= 'Z')
    || (c >= 'a' && c <= 'z')
    || (c >= '0' && c <= '9')
    || (c == '-')) {
        return 2;
    } else if ((c == '\n')
    || (c == '\t')
    || (c == ' ')) {
        return 1;
    }
    return 0;
}

ErrorCode completeTask(FILE *input, FILE *output)
{
    if (input == NULL || output == NULL) {
        return ERROR_INVALID_FILE_POINTER;
    }

    char c;
    char lastUselessChar = 0;
    char base = '2';
    short int sign = 1;

    char buff[1024];
    buff[0] = 0;
    buff[1] = 0;
    short int currCharPosition = 0;

    bool numberStarted = false;

    while ((c = fgetc(input)) != EOF) {
        if (symbolType(c) == 0) {
            if (fprintf(output, "\nНеопознанный символ -> %c", c) <= 0) {
                return ERROR_CANT_WRITE_ERROR;
            }
            return ERROR_INVALID_VALUE_IN_FILE;
        }

        c = toupper(c);

        if (c == '-') {
            buff[currCharPosition++] = c;
            sign = -1;
            continue;
        }

        if (!numberStarted && symbolType(c) == 1 && sign == -1) {
            if (fprintf(output, "\nПосле знака минус отсутсвуют цифры.") <= 0) {
                return ERROR_CANT_WRITE_ERROR;
            }
            return ERROR_INVALID_VALUE_IN_FILE;
        }

        if (!numberStarted && symbolType(c) == 2 && c != '0') {
            if (buff[0] != 0) {
                fprintf(output, "\n");
            }
            if (fprintf(output, "Число: ") <= 0) {
                return ERROR_OUTPUT_FILE_ERROR;
            }
            
            numberStarted = true;
            buff[currCharPosition++] = c;
            base = c;
            continue;
        }

        if (numberStarted && symbolType(c) == 2) {
            buff[currCharPosition++] = c;
            if (c > base) {
                base = c;
            }
            continue;
        }

        if (symbolType(c) == 1 && lastUselessChar == '0' && !numberStarted) {
            if (buff[0] != 0) {
                fprintf(output, "\n");
            }

            buff[0] = '0';

            if (fprintf(output, "Число: 0") <= 0) {
                return ERROR_OUTPUT_FILE_ERROR;
            }

            if (fprintf(output, "\nОснование системы счисления: 2\nДесятичное представление: 0") <= 0) {
                return ERROR_OUTPUT_FILE_ERROR;
            }
        }

        if (numberStarted && symbolType(c) == 1) {            
            double value = 0;
            base = 1 + (isdigit(base) ? base -'0' : base -'A'+ 10);

            for (int i = 0; i < currCharPosition; i++) {
                if (fprintf(output, "%c", buff[i]) <= 0) {
                    return ERROR_OUTPUT_FILE_ERROR;
                }
            }

            ErrorCode errors = translateNumber(buff, currCharPosition, sign, base, &value, output);
            if (errors) {
                return errors;
            }

            if (fprintf(output, "\nОснование системы счисления: %d\nДесятичное представление: %.0lf", base, value) <= 0) {
                return ERROR_OUTPUT_FILE_ERROR;
            }

            base = '2';
            currCharPosition = 0;
            lastUselessChar = 0;
            numberStarted = false;
            sign = 1;
            continue;
        }

        lastUselessChar = c;
    }

    if (buff[0] == '-' & buff[1] == 0) {
        if (fprintf(output, "\nНеопознанный символ -> %c", buff[0]) <= 0) {
            return ERROR_CANT_WRITE_ERROR;
        }
        return ERROR_INVALID_VALUE_IN_FILE;
    }

    if (numberStarted) {
        double value = 0;
        base = 1 + (isdigit(base) ? base -'0' : base -'A'+ 10);

        for (int i = 0; i < currCharPosition; i++) {
            if (fprintf(output, "%c", buff[i]) <= 0) {
                return ERROR_OUTPUT_FILE_ERROR;
            }
        }

        ErrorCode errors = translateNumber(buff, currCharPosition, sign, base, &value, output);
        if (errors) {
            return errors;
        }

        if (fprintf(output, "\nОснование системы счисления: %d\nДесятичное представление: %.0lf", base, value) <= 0) {
            return ERROR_OUTPUT_FILE_ERROR;
        }
    }

    if (lastUselessChar == '0') {
        if (buff[0] != 0) {
            fprintf(output, "\n");
        }

        buff[0] = '0';

        if (fprintf(output, "Число: 0") <= 0) {
            return ERROR_OUTPUT_FILE_ERROR;
        }

        if (fprintf(output, "\nОснование системы счисления: 2\nДесятичное представление: 0") <= 0) {
            return ERROR_OUTPUT_FILE_ERROR;
        }
    }

    return SUCCESS;
}