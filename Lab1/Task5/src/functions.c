#include "functions.h"
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

// Локальные функции

short int charLength(char byte)
{
    if  ((byte & 0x80) == 0) {
        return 1;
    } 
    if  ((byte & 0xE0) == 0xC0) {
        return 2;
    } 
    if  ((byte & 0xF0) == 0xE0) {
        return 3;
    } 
    if  ((byte & 0xF8) == 0xF0) {
        return 4;
    } 
    return 0;
}

bool isAcceptableLetter(const char c)
{
    return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

// И ещё проверка файлов!

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

// The важные функции 

ErrorCode digitRemove(FILE *input, FILE *output)
{
    if (input == NULL || output == NULL) {
        return ERROR_INVALID_FILE_POINTER;
    }
    char c;

    while ((c = fgetc(input)) != EOF) {
        if (!isdigit(c)) {
            if (fputc(c, output) == EOF) {
                return ERROR_OUTPUT_FILE_ERROR;
            }
        }
    }

    return SUCCESS; 
}

ErrorCode letterCount(FILE *input, FILE *output)
{
    if (input == NULL || output == NULL) {
        return ERROR_INVALID_FILE_POINTER;
    }

    int c;
    unsigned int counter = 0;

    while ((c = fgetc(input)) != EOF) {
        if (c == '\n') {
            if (fprintf(output, "%d \n", counter) <= 0) {
                return ERROR_OUTPUT_FILE_ERROR;
            }
            counter = 0;
        } else if (isAcceptableLetter(c)) {
            counter++;
        }
    }

    if (counter > 0) {
        if (fprintf(output, "%d \n", counter) <= 0) {
            return ERROR_OUTPUT_FILE_ERROR;
        }
    }

    return SUCCESS;
}

ErrorCode countEveryoneElse(FILE *input, FILE *output)
{
    if (input == NULL || output == NULL) {
        return ERROR_INVALID_FILE_POINTER;
    }

    int pureChar;
    unsigned int counter = 0;
    short int length;

    while ((pureChar = fgetc(input)) != EOF) {
        unsigned char c = (unsigned char)pureChar;
        
        if (c == '\n') {
            if (fprintf(output, "%d \n", counter) <= 0) {
                return ERROR_OUTPUT_FILE_ERROR;
            }
            counter = 0;

        } else {

            if (!isAcceptableLetter(c) && !isdigit(c) && !isspace(c)) {
                counter++;
                length = charLength(c);

                if (length == 0) {
                    return ERROR_FILE_CORRUPTED;
                }

                for (int i = 1; i < length; i++) {
                    fgetc(input);
                }
            }
        }
    }

    if (counter > 0) {
        if (fprintf(output, "%d \n", counter) <= 0) {
            return ERROR_OUTPUT_FILE_ERROR;
        }
    }

    return SUCCESS;
}

ErrorCode hexReplace(FILE *input, FILE *output)
{
    if (input == NULL || output == NULL) {
        return ERROR_INVALID_FILE_POINTER;
    }

    int pureChar;

    while ((pureChar = fgetc(input)) != EOF) {
        unsigned char c = (unsigned char)pureChar;

        if (isdigit(c) || c == '\n') {
            fputc(c, output);

        } else {
            int firstDigit = c / 16;
            int secondDigit = c % 16;

            char firstChar = firstDigit < 10 ? '0' + firstDigit : 'A' + firstDigit - 10;
            char secondChar = secondDigit < 10 ? '0' + secondDigit : 'A' + secondDigit - 10;

            if (fputc(firstChar, output) == EOF || fputc(secondChar, output) == EOF) {
                return ERROR_OUTPUT_FILE_ERROR;
            }
        }        
    }

    return SUCCESS;
}