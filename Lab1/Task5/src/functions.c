#include "functions.h"
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

bool isAcceptableLetter(const char c) {
    return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

short int digitRemove(FILE *input, FILE *output) {
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

short int letterCount(FILE *input, FILE *output) {
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

short int countEveryoneElse(FILE *input, FILE *output) {
    if (input == NULL || output == NULL) {
        return 1;
    }

    char line[1024];

    while (fgets(line, 1024, input) != NULL) {
        unsigned int counter = 0;

        for (int i = 0; line[i] != '\0'; i++) {
            unsigned char ch = line[i];
            if (!isAcceptableLetter(line[i]) && !isdigit(line[i]) && !isspace(line[i])) {
                counter++;
            }
        }

        fprintf(output, "%d\n", counter);
    }

    return 0;
}