#include "functions.h"

#include <stdlib.h>

int check_brackets(const char *str) {
    if (str == NULL) {
        return -1;
    }

    size_t maxSize = strlen(str);
    if (maxSize == 0) {
        return 0;
    }

    char *stack = (char *)malloc(maxSize * sizeof(char));
    if (stack == NULL) {
        return -1;
    }

    int result = 0;

    char openings[] = "({<[";
    char endings[] = ")}>]";

    short int top = -1;

    for (int currPos = 0; str[currPos] != '\0'; currPos++) {
        char c = str[currPos];

        if (strchr(openings, c) != NULL) {
            stack[++top] = c;

        } else if (strchr(endings, c) != NULL) {
            if (top == -1) {
                result = 1;
                break;
            }

            char* corrEnd = strchr(endings, c);
            short int index = corrEnd - endings;

            if (stack[top] != openings[index]) {
                result = 1;
                break;
            }

            top--;
        }
    }

    if (result == 0 && top != -1) {
        result = 1;
    }
    
    free(stack);
    return result;
}