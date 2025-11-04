#include "functions.h"

#include <stdbool.h>
#include <ctype.h>
#include <math.h>

// Проверка файла на валидность
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

// Определитель длины в байтах
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

// Является ли байт разделителем?
short int separatorCheck(int c)
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

// Количество символов (не байтов)
size_t amountOfChars(const char *str, size_t bytes)
{
    size_t symbolsCount = 0;
    size_t i = 0;

    while (i < bytes && str[i] != '\0') {

        short int length = charLength(str[i]);

        if (length == 0) {
            break;
        }

        if (i + length < bytes) {
            symbolsCount++;
            i += length;
        } else {
            break;
        }
    }

    return symbolsCount;
}

// Поиск байтовой позиции 80ой буквы
size_t findBreak(const char *line, size_t start, size_t maxChars)
{
    size_t bytePos = start;
    size_t charCount = 0;

    while (bytePos < strlen(line) && charCount < maxChars) {

        short length = charLength(line[bytePos]);

        if (length == 0 || bytePos + length > strlen(line)) {
            break;
        }

        bytePos += length;
        charCount++;
    }

    return bytePos;
}

// Форматирование подстроки
ErrorCode formatLine(char *line, size_t byteLength, FILE *output)
{
    if (byteLength == 0) {
        return SUCCESS;
    }

    size_t lineCharLen = amountOfChars(line, byteLength);
    size_t wordCount = 0;
    int inWord = 0;
    size_t i = 0;

    while (i < byteLength) {

        short int length = charLength(line[i]);

        if (length == 0) {
            i++;
            continue;
        }

        if (separatorCheck(line[i])) {
            inWord = 0;
        } else {

            if (!inWord) {
                wordCount++;
                inWord = 1;
            }
        }

        i += length;
    }

    if (wordCount <= 1) {

        size_t charCount = 0;
        size_t endPos = 0;

        while (endPos < byteLength && charCount < 80) {
            short int length = charLength(line[endPos]);

            if (length == 0) {
                break;
            }

            endPos += length;
            charCount++;
        }

        char buff[321];
        strncpy(buff, line, endPos);
        buff[endPos] = '\0';

        size_t firstCharPos = 0;
        while (firstCharPos < endPos) {
            short int length = charLength(buff[firstCharPos]);

            if (length > 0) {

                if (fprintf(output, "%s\n", buff + firstCharPos) < 0) {
                    return ERROR_INVALID_OUTPUT;
                }

                return SUCCESS;
            }

            firstCharPos++;
        }

        return ERROR_INVALID_OUTPUT;
    }

    size_t totalSpaces = 80 - lineCharLen + (wordCount - 1);
    size_t neededSpaces = totalSpaces - wordCount - 1;
    size_t bonusSpaces = 1 + neededSpaces / (wordCount - 1);
    size_t freeSpaces = 1 + neededSpaces % (wordCount - 1);

    char *reLine = malloc(321);

    if (reLine == NULL) {
        return ERROR_MEMORY_ALLOCATION;
    }

    size_t rePos = 0;
    size_t linePos = 0;
    int currWord = 0;
    inWord = 0;

    while (linePos < byteLength && rePos < 320) {
        short int length = charLength(line[linePos]);

        if (length == 0) {
            linePos++;
            continue;
        }

        if (separatorCheck(line[linePos])) {
            
            if (inWord) {
                currWord++;

                if (currWord == wordCount) {
                    break;
                }

                for (size_t i = 0; i < bonusSpaces; i++) {
                    reLine[rePos++] = ' '; 
                }

                if (freeSpaces > 0) {
                    reLine[rePos++] = ' ';
                    freeSpaces -= 1;
                }

                inWord = 0;
            }

            linePos++;
        } else {

            for (int j = 0; j < length; j++) {
                reLine[rePos++] = line[linePos + j];
            }

            linePos += length;
            inWord = 1;
        }
    }

    reLine[rePos] = '\0';
    size_t firstCharPos = 0;
    
    while (firstCharPos < rePos) {
        short int length  = charLength(reLine[firstCharPos]);

        if (length > 0) {
            
            if (fprintf(output, "%s\n", reLine + firstCharPos) < 0) {

                free(reLine);
                return ERROR_INVALID_OUTPUT;
            }

            free(reLine);
            return SUCCESS;
        }

        firstCharPos++;
    }

    free(reLine);

    return ERROR_INVALID_OUTPUT;
}

// Первоначальная обработка длинных строк: выкидываем лишние слова,
// разделяем на меньшие строки по 80- символов
ErrorCode convertLongLine(const char *line, size_t byteLength, FILE *output)
{
    size_t start = 0;

    while (start < byteLength) {
        size_t lastByte = start;
        size_t lastSpace = start;
        size_t charCount = 0;
        bool idealStr = false;

        while (lastByte < byteLength && charCount < 80) {
            const char *currPos = line + lastByte;
            short int currCharLength = charLength(*currPos);

            if (currCharLength == 0 || lastByte + currCharLength > byteLength) {
                break;
            }

            if (separatorCheck(line[lastByte])) {
                lastSpace = lastByte;
            }

            lastByte += currCharLength;
            charCount++;
        }

        if (lastByte < byteLength && charCount == 80) {
            const char *nextCharPos = line + lastByte;
            short int nextCharLength = charLength(*nextCharPos);

            if (separatorCheck(line[lastByte])) {
                idealStr = true;
                lastByte += nextCharLength;
            }
        }

        if (lastByte >= byteLength || charCount < 80 || idealStr) {
            size_t outputLength = lastByte - start;

            if (outputLength > 0) {

                char *outputLine = malloc(outputLength + 1);
                if (outputLine == NULL) {
                    return ERROR_MEMORY_ALLOCATION;
                }

                strncpy(outputLine, line + start, outputLength);

                if (idealStr) {
                    outputLength--;
                }
                outputLine[outputLength] = '\0';

                if (fprintf(output, "%s\n", outputLine) < 0) {
                    free(outputLine);
                    return ERROR_INVALID_OUTPUT;
                }
                free(outputLine);

            } else {
                return ERROR_INVALID_OUTPUT;
            }

            start = lastByte;
            continue;
        }

        size_t breakPos;
        
        if (lastSpace > start) {
            breakPos = lastSpace;
        } else {
            breakPos = lastByte;
        }

        size_t childLineLength = breakPos - start;
        char *childLine = malloc(childLineLength + 1);

        if (childLine == NULL) {
            return ERROR_MEMORY_ALLOCATION;
        }

        strncpy(childLine, line + start, childLineLength);
        childLine[childLineLength] = '\0';

        ErrorCode result = formatLine(childLine, childLineLength, output);
        
        free(childLine);

        if (result != SUCCESS) {
            return result;
        }

        start = breakPos;

        while (start < byteLength) {
            short int length = charLength(line[start]);

            if (length != 1 || !separatorCheck(line[start])) {
                break;
            }

            start++;
        }
    }

    return SUCCESS;
}

// Основная функция
ErrorCode completeTask(FILE *input, FILE *output)
{
    if (input == NULL || output == NULL) {
        return ERROR_INVALID_FILE_POINTER;
    }

    char *line = NULL;
    size_t lineSize = 0;
    int read;

    while ((read = getline(&line, &lineSize, input)) != -1) {

        if (read > 0 && line[read - 1] == '\n') {
            line[read - 1] = '\0';
            read--;
        }

        size_t strLength = amountOfChars(line, read);

        if (strLength <= 80) {
            if (fprintf(output, "%s\n", line) < 0) {
                free(line);
                return ERROR_INVALID_OUTPUT;
            }
            continue;
        }

        ErrorCode result = convertLongLine(line, read, output);

        if (result != SUCCESS) {
            free(line);
            return result;
        }
    }

    free(line);

    return SUCCESS;
}
