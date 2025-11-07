#include "functions.h"
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// Конструкция, ведущая себя как поток из файла и строка одновременно.
// (в зависимости от надобности)

typedef struct {
    FILE *file;
    const char **str;
} universalInput;

int getter(universalInput *in) {
    if (in->file) {
        return fgetc(in->file);
    }
    if (in->str && **in->str != '\0') {
        char c = **in->str;
        (*in->str)++;
        return (int)c;
    }
    return EOF;
}

int ungetter(int c, universalInput *in) {
    if (c == EOF) {
        return EOF;
    }
    if (in->file) {
        return ungetc(c, in->file);
    }
    if (in->str && *in->str != NULL) {
        (*in->str)--;
        return c;
    }
    return EOF;
}

// Типы флагов

typedef enum {
    OTHER,
    ROMAN, // %Ro
    ZECKENDORF, // %Zr
    LOWER, // %Cv
    UPPER, // %CV

} FlagType;

// Вспомогательныя функции

int getValue(char c, bool capitalize)
{
    c = capitalize ? toupper(c) : tolower(c);

    if (c >= '0' && c <= '9') {
        return c - '0';
    }
    if (c >= 'a' && c <= 'z') {
        return c - 'a' + 10;
    }
    if (c >= 'A' && c <= 'Z') {
        return c - 'A' + 10;
    }
    return -1;
}

int getRomanValue(char c)
{
    switch (toupper(c)) {
        case 'I':
            return 1;
        case 'V':
            return 5;
        case 'X':
            return 10;
        case 'L':
            return 50;
        case 'C':
            return 100;
        case 'D':
            return 500;
        case 'M':
            return 1000;
        default:
            return -1;
    }
}

unsigned int getFibonacciValue(int n)
{
    if (n <= 0 || n > 47) {
        return 0;
    }
    if (n == 1) {
        return 1;
    }
    if (n == 2) {
        return 2;
    }
    
    unsigned int a = 1, b = 2;
    for (int i = 3; i <= n; i++) {
        unsigned int temp = a + b;
        a = b;
        b = temp;
    }
    return b;
}

// Фунции пунктов

int zeckendorfFlag(universalInput *in, unsigned int *out)
{
    char buff[48];
    int count = 0;
    int c;
    
    while (isspace(c = getter(in))) {
        if (c == EOF) {
            return 0;
        }
    }

    while (c != EOF && count < 47) {
        
        if (c == '0') {
            buff[count++] = c;
            c = getter(in);

        } else if (c == '1') {

            buff[count++] = c;
            c = getter(in);
            
            if (c == EOF || isspace(c)) {
                if (c != EOF) {
                    ungetter(c, in);
                }
                break;
            }

        } else {
            ungetter(c, in);
            return 0;
        }
    }
    
    if (count == 0) {
        return 0;
    }
    
    unsigned int result = 0;
    
    for (int i = 0; i < count; i++) {
        if (buff[i] == '1') {
            result += getFibonacciValue(count - i);
        }
    }
    
    *out = result;
    return 1;
}

int romanFlag(universalInput *in, int *out)
{
    char buff[32];
    int count = 0;
    char c;
    
    while (isspace(c = getter(in))) {
        if (c == EOF) {
            return 0;
        }
    }

    while (c != EOF && count < 31) {
        if (getRomanValue(c) == -1) {
            printf("Ouch!");
            ungetter(c, in);
            break;
        }

        buff[count++] = c;
        c = getter(in);
    }
    
    if (count == 0) {
        if (c != EOF) {
            ungetter(c, in);
        }
        return 0; 
    }
    
    if (c != EOF) {
        ungetter(c, in);
    }

    int otv = 0;
    for (int i = 0; i < count; i++) {
        int numOne = getRomanValue(buff[i]);
        int numTwo = (i + 1 < count) ? getRomanValue(buff[i+1]) : 0;
        if (numOne < numTwo) {
            otv += (numTwo - numOne);
            i++;
        } else {
            otv += numOne;
        }
    }

    if (otv <= 0 || otv > 3999) {
        return 0;
    }

    *out = otv;

    return 1; 
}

int baseFlag(universalInput *in, int *out, int base, bool capitalize)
{
    if (base < 2 || base > 36) {
        base = 10;
    }
    
    int c;
    long long otv = 0;
    int sign = 1;
    bool empty = true;

    while (isspace(c = getter(in))) {
        if (c == EOF) {
            return 0;
        }
    }

    if (c == '-') {
        sign = -1;
        c = getter(in);
    }

    while (c != EOF && !isspace(c)) {
        int value = getValue(c, capitalize);

        if (value < 0 || value >= base) {
            ungetter(c, in);
            break;
        }

        empty = false;

        if ((otv > (2147483647 / base)) || (otv < 0)) {
            while (c != EOF && !isspace(c)) {
                c = getter(in);
            }
            if (c != EOF) {
                ungetter(c, in);
            }

            *out = 0;
            return 1;
        }
        
        otv = otv * base + value;
        c = getter(in);
    }
    
    if (c != EOF) {
        ungetter(c, in);
    }

    if (empty) {
        return 0;
    }

    *out = (int)(otv * sign);
    return 1;
}

// Функция анализа флагов

FlagType flagTyper(const char* format)
{
    const char* curr = format;
    
    if (*curr == '\0' || *(curr + 1) == '\0') {
        return OTHER;
    }
    
    if (curr[0] == 'R' && curr[1] == 'o') {
        return ROMAN;
    }
        
    if (curr[0] == 'Z' && curr[1] == 'r') {
        return ZECKENDORF;
    }

    if (curr[0] == 'C') {
        if (curr[1] == 'v') 
            return LOWER;
        if (curr[1] == 'V') 
            return UPPER;
    }

    return OTHER;
}

// Для нормальных флагов

int fileBoringFlag(FILE *stream, const char **format, va_list *args)
{
    const char *currFormat = *format;
    char token[512];
    int tokenLen = 0;
    int result = 0;
    int curr;
    
    while ((curr = fgetc(stream)) != EOF && isspace(curr)) {
    }
    
    if (curr == EOF) {
        return 0;
    }
    
    token[tokenLen++] = curr;
    while ((curr = fgetc(stream)) != EOF && !isspace(curr) && tokenLen < 511) {
        token[tokenLen++] = curr;
    }
    token[tokenLen] = '\0';
    
    if (tokenLen == 0 || tokenLen == 511) {
        return 0;
    }
    
    char *endPos = NULL;
    
    switch (*currFormat) {
        case 'd': {
            long value = strtol(token, &endPos, 10);
        
            if (endPos != token && *endPos == '\0') {

                int *output = va_arg(*args, int*);
                *output = (int)value;

                result = 1;
            }
            break;
        }
        
        case 'u': {
            unsigned long value = strtoul(token, &endPos, 10);
            
            if (endPos != token && *endPos == '\0') {

                unsigned int *output = va_arg(*args, unsigned int*);
                *output = (unsigned int)value;

                result = 1;
            }
            break;
        }
        
        case 'x': {
            unsigned long value = strtoul(token, &endPos, 16);
            
            if (endPos != token && *endPos == '\0') {

                unsigned int *output = va_arg(*args, unsigned int*);
                *output = (unsigned int)value;

                result = 1;
            }
            break;
        }
        
        case 'f': {
            double value = strtod(token, &endPos);
            
            if (endPos != token && *endPos == '\0') {

                float *output = va_arg(*args, float*);
                *output = (float)value;

                result = 1;
            }
            break;
        }
        
        case 'l': {
            if (currFormat[1] == 'f') {
                double value = strtod(token, &endPos);
                
                if (endPos != token && *endPos == '\0') {

                    double *output = va_arg(*args, double*);
                    *output = value;

                    result = 1;
                }

                (*format)++;
            }
            break;
        }
        
        case 's': {
            char *output = va_arg(*args, char*);

            strcpy(output, token);

            result = 1;
            break;
        }
    }
    
    if (result > 0) {
        (*format)++;
        char trash;
        ungetc(trash, stream);
    }
    
    return result;
}

int strBoringFlag(const char **str, const char **format, va_list *args)
{
    const char *currFormat = *format;
    const char *inputStr = *str;
    const char *strStart = inputStr;
    int result = 0;

    while (isspace((unsigned char)*inputStr)) {
        inputStr++;
    }
    
    if (*inputStr == '\0') {
        return 0;
    }
    
    const char *tokenStart = inputStr;
    const char *tokenEnd = inputStr;
    while (*tokenEnd != '\0' && !isspace((unsigned char)*tokenEnd)) {
        tokenEnd++;
    }
    
    size_t tokenLen = tokenEnd - tokenStart;

    if (tokenLen == 0) {
        return 0;
    }
    
    char *token = (char *)malloc(tokenLen + 1);
    if (!token) {
        return -1;
    }

    strncpy(token, tokenStart, tokenLen);
    token[tokenLen] = '\0';

    char *endPos = NULL;
    
    
    switch (*currFormat) {
        case 'd': {
            long value = strtol(token, &endPos, 10);
            
            if (endPos != token && *endPos == '\0') {

                int *output = va_arg(*args, int*);
                *output = (int)value;

                result = 1;
            }
            break;
        }
        
        case 'u': {
            unsigned long value = strtoul(token, &endPos, 10);
            
            if (endPos != token && *endPos == '\0') {

                unsigned int *output = va_arg(*args, unsigned int*);
                *output = (unsigned int)value;

                result = 1;
            }
            break;
        }
        
        case 'x': {
            unsigned long value = strtoul(token, &endPos, 16);
            
            if (endPos != token && *endPos == '\0') {

                unsigned int *output = va_arg(*args, unsigned int*);
                *output = (unsigned int)value;

                result = 1;
            }
            break;
        }
        
        case 'f': {
            double value = strtod(token, &endPos);
            
            if (endPos != token && *endPos == '\0') {

                float *output = va_arg(*args, float*);
                *output = (float)value;

                result = 1;
            }
            break;
        }
        
        case 'l': {
            if (currFormat[1] == 'f') {
                double value = strtod(token, &endPos);
                
                if (endPos != token && *endPos == '\0') {

                    double *output = va_arg(*args, double*);
                    *output = value;

                    result = 1;
                }

                (*format)++;
            }
            break;
        }
        
        case 's': {
            char *output = va_arg(*args, char*);

            result = 1;

            break;
        }
    }
    
    free(token);
    
    if (result > 0) {
        (*format)++;
        *str = tokenEnd;
    } else {
        *str = strStart;
    }
    
    return result;
}

// Распределитель

int completeFlag(FlagType flag, universalInput *in, va_list *args)
{
    switch (flag) {
        case ROMAN:
            return romanFlag(in, va_arg(*args, int*));
        case ZECKENDORF:
            return zeckendorfFlag(in, va_arg(*args, unsigned int*));
        case LOWER:
        case UPPER:
            return baseFlag(in, va_arg(*args, int*), va_arg(*args, int), (flag == UPPER));
        default:
            return -1;
    }
}

// Основные функции

int overfscanf(FILE * stream, const char *format, ...)
{
    va_list args;
    va_start(args, format);

    const char *curr = format;
    int written = 0;

    universalInput in;
    in.file = stream;
    in.str = NULL;

    while (*curr != '\0') 
    {
        if (*curr == '%') 
        {
            curr++;
            
            if (*curr == '%') {
                int c = getter(&in);
                curr++;
                continue;
            }
            
            FlagType typeOfFlag = flagTyper(curr);
            int currWritten = 0;
            
            if (typeOfFlag == OTHER) {
                // Нормальные флаги
                currWritten = fileBoringFlag(stream, &curr, &args);

            } else {
                curr+= 2;
                // Обработка кастомных флагов
                currWritten = completeFlag(typeOfFlag, &in, &args);
            }
            
            if (currWritten > 0) {
                written += currWritten;
            } else {
                va_end(args);
                return written;
            }

        } else {

            int c = getter(&in);
            curr++;
        }
    }

    va_end(args);
    return written;
}

int oversscanf(const char *str, const char *format, ...)
{
    va_list args;
    va_start(args, format);

    const char *curr = format;
    int written = 0;
    
    universalInput in;
    in.file = NULL;
    in.str = &str;

    while (*curr != '\0') 
    {
        if (*curr == '%') 
        {
            curr++;
            
            if (*curr == '%') {
                int c = getter(&in);
                if (c != '%') {
                    if (c != EOF) ungetter(c, &in);
                    va_end(args);
                    return written;
                }
                curr++;
                continue;
            }
            
            FlagType typeOfFlag = flagTyper(curr);
            int currWritten = 0;
            
            if (typeOfFlag == OTHER) {
                // Нормальные флаги
                currWritten = strBoringFlag(&str, &curr, &args);

            } else {
                curr+= 2;
                // Обработка кастомных флагов
                currWritten = completeFlag(typeOfFlag, &in, &args);
            }
            
            if (currWritten > 0) {
                written += currWritten;
            } else {
                va_end(args);
                return written;
            }

        } else {

            int c = getter(&in);
            curr++;
        }
    }

    va_end(args);
    return written;
}