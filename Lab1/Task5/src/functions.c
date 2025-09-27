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

// И ещё проверка файлов

short int isThatFileGood(const char *name)
{
    if (name == NULL) {
        return 1;
    }
    const char *fileTypes[] = {".txt", ".docx", "doc", "rtf", ".c", ".h", ".cpp", ".hpp", ".java", ".py", ".csv", ".json", ".xml", ".html", ".css", ".js", ".md", ".log", ".conf", ".config", NULL};

    char *splitter = strrchr(name, '.');

    if (splitter == NULL) {
        return 2;
    }

    for (int i = 0; fileTypes[i] != NULL; i++) {
        if (strcmp(splitter, fileTypes[i]) == 0) {
            return 0;
        }
    }

    return 3;
}

// The важные функции 

short int digitRemove(FILE *input, FILE *output)
{
    if (input == NULL || output == NULL) {
        return 1;
    }
    char c;

    while ((c = fgetc(input)) != EOF) {
        if (!isdigit(c)) {
            fputc(c, output);
        }
    }

    return 0;
}

short int letterCount(FILE *input, FILE *output)
{
    if (input == NULL || output == NULL) {
        return 1;
    }

    char line[1024];

    while (fgets(line, 1024, input) != NULL) { // длинная строка
        unsigned int counter = 0;

        for (int i = 0; line[i] != '\0'; i++) {
            if (isAcceptableLetter(line[i])) {
                counter++;
            }
        }

        fprintf(output, "%d\n", counter);
    }

    return 0;
}

short int countEveryoneElse(FILE *input, FILE *output)
{
    if (input == NULL || output == NULL) {
        return 1;
    }

    char line[1024];

    while (fgets(line, 1024, input) != NULL) {
        unsigned int counter = 0;
        int i = 0;
        while (line[i] != '\0') {
            unsigned char ch = line[i];
            short int length = charLength(ch);

            if (!isAcceptableLetter(ch) && !isdigit(ch) && !isspace(ch)) {
                counter++;
            }

            i += length;
        }

        fprintf(output, "%d\n", counter);
    }

    return 0;
}

short int hexReplace(FILE *input, FILE *output)
{
    if (input == NULL || output == NULL) {
        return 1;
    }

    char c;

    while ((c = fgetc(input)) != EOF) {

        if (isdigit(c)) {
            fputc(c, output);
        } else {
            int firstLetter = c / 16;
            int secondLetter = c % 16;

            fputc(firstLetter < 10 ? '0' + firstLetter : 'A' + firstLetter - 10, output);
            fputc(secondLetter < 10 ? '0' + secondLetter : 'A' + secondLetter - 10, output);
        }        
    }

    return 0;
}