#include "functions.h"

#include <stdbool.h>

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

bool isThatAllowed(char c) {
    if ((c >= 'A' && c <= 'Z')
    || (c >= 'a' && c <= 'z')
    || (c == '\n')
    || (c == '\0')
    || (c == ' ')) {
        return 1;
    }
    return 0;
}

ErrorCode func(FILE *input, FILE *output)
{
    if (input == NULL || output == NULL) {
        return ERROR_INVALID_FILE_POINTER;
    }

    char c;

    while ((c = fgetc(input)) != EOF) {
        if (!isThatAllowed(c)) {
            return ERROR_INVALID_VALUE_IN_FILE;
        }

        printf("%c", c);

        
    }
}

// Пригодится.
// char* p = st;
// while (*p) {
//     res = res * base + (isdigit(*p)? *p -'0' : *p -'A'+ 10 );
//     p++;
// }

// Тоже возможно.
// char** Array = NULL;
// char buff[BUFSIZ];
// int count = 0;
// int i = 0;
// while (1) {
// 	gets(buff)
// 	if (!strcmp(buff, "stop")) { break; }
// 	index++;
// 	if (index >= capacity) {
// 		Array** [index] = malloc(strlen(buff) + 1);
// 		strcpy(Array[index], buff);
// 	}
// 	else {
// 		Arrays = realloc(Arrays, capacity *= 2);
// 	}
// 	++index;
// }
// int i;
// for (i = 0, i < index, i++) {
// 	printf("%s \n", Arrays[i]);
// 	free(Arrays[i]);
// }
// free(Arrays);