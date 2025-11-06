#include "functions.h"

int check_brackets(const char *str) {
    if (str == NULL) {
        return -1;
    }

    char stack[1024];

    char openings[] = "({<[";
    char endings[] = ")}>]";

    short int top = -1;

    for (int currPos = 0; str[currPos] != '\0'; currPos++) {
        char c = str[currPos];

        if (strchr(openings, c) != NULL) {
            if (top >= 1023) {
                return -1;
            }

            stack[++top] = c;

        } else if (strchr(endings, c) != NULL) {
            if (top == -1) {
                return -1;
            }

            char* corrEnd = strchr(endings, c);
            short int index = corrEnd - endings;

            if (stack[top] != openings[index]) {
                return 1;
            }

            top--;
        }
    }

    if (top != -1) {
        return 1;
    }
    
    return 0;
}