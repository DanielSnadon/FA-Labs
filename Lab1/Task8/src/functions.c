#include "functions.h"
#include <ctype.h>
#include <stdlib.h>

ErrorCode clearBuffer()
{
    char c;
    while ((c = getchar()) != '\n' && c != '\0')
    {
        if (c != ' ') {
            return ERROR_INVALID_INPUT;
        }
    }
    return SUCCESS;
}

ErrorCode translate(char* line, const short int base, long long* maxNumber) {
    if (line == NULL || maxNumber == NULL) {
        return ERROR_NULL_POINTER;
    }

    long long answer = 0;
    const char* curChar = line;

    while (*curChar == ' ' || *curChar == '\t') {
        curChar++;
    }

    if (*curChar == '\0' || *curChar == '\n') {
            return ERROR_EMPTY_LINE;
    }

    short int sign = 1;
    if (*curChar == '-') {
        sign = -1;
        curChar++;
        if (*curChar == '\0' || *curChar == '\n' || *curChar == ' ' || *curChar == '\t') {
            return ERROR_EMPTY_LINE;
        }
    }

    while (*curChar != '\n' && *curChar != '\0') {
        char c = *curChar;
        short int digit;
        if (isdigit(c)) {
            digit = c - '0';
        } else if (isalpha(c)) {
            digit = c - 'A' + 10;
        } else {
            return ERROR_INVALID_INPUT;
        }

        if (digit >= base) {
            return ERROR_INVALID_INPUT;
        }

        if (answer > ((__LONG_LONG_MAX__ - digit) / base) || answer < 0) {
            return ERROR_OVERFLOW;
        }
        answer = answer * base + digit;
        curChar++;
    }

    answer *= sign;

    if (llabs(answer) > llabs(*maxNumber)) {
        *maxNumber = answer;
    }

    return SUCCESS;
}

ErrorCode rebase(const long long number, const short int base, char* answer, const short int maxSize) {
    if (answer == NULL) {
        return ERROR_NULL_POINTER;
    }

    if (base < 0 || base > 36) {
        return ERROR_INVALID_INPUT;
    }

    long long alive = (number < 0) ? -number : number;
    short int index = 0;

    if (alive == 0) {
        answer[index++] = '0';
        answer[index] = '\0';
        return SUCCESS;
    }

    while (alive > 0 && index < maxSize) {
        short int digit = alive % base;
        alive /= base;

        answer[index++] = (digit > 9) ? 'A' + digit - 10 : '0' + digit;
    }

    if (alive > 0) {
        return ERROR_OVERFLOW;
    }

    if (number < 0) {
        answer[index++] = '-';
    }

    for (int i = 0; i < index / 2; i++) {
        char t = answer[i];
        answer[i] = answer[index - 1 - i];
        answer[index - 1 - i] = t;
    }

    answer[index] = '\0';

    return SUCCESS;
}