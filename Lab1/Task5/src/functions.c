#include "functions.h"
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

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

    while (fgets(line, 1024, input) != NULL) {
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

