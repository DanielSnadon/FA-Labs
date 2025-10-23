#include "functions.h"
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    OTHER,
    ROMAN, // %Ro
    ZECKENDORF, // %Zr
    LOWER, // %Cv
    UPPER, // %CV

} FlagType;

// Вспомогательныя функции

int getValue(char c, bool capitalize) {
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

int getRomanValue(char c) {
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

static unsigned int getFibonacciValue(int n)
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

int zeckendorfFlag(FILE *stream, unsigned int *output)
{
    char buff[48];
    int count = 0;
    char c;
    
    while (isspace(c = fgetc(stream))) {
        if (c == EOF) {
            return 0;
        }
    }

    while (c != EOF && count < 47) {
        
        if (c == '0') {
            buff[count++] = c;
            c = fgetc(stream);

        } else if (c == '1') {

            buff[count++] = c;
            c = fgetc(stream);
            
            if (c == EOF || isspace(c)) {
                if (c != EOF) {
                    ungetc(c, stream);
                }
                break;
            }

        } else {
            ungetc(c, stream);
            return 0;
        }
    }
    
    if (count == 0) {
        return 0;
    }
    
    unsigned int result = 0;
    
    for (int i = 0; i < count - 1; i++) {
        if (buff[i] == '1') {
            result += getFibonacciValue(i + 1);
        }
    }
    
    *output = result;
    return 1;
}

int romanFlag(FILE *stream, int *output)
{
    char buff[32];
    int count = 0;
    char c;
    
    while (isspace(c = fgetc(stream))) {
        if (c == EOF) {
            return 0;
        }
    }

    while (c != EOF && count < 31) {
        if (getRomanValue(c) == 0) {
            ungetc(c, stream);
            break;
        }

        buff[count++] = c;
        c = fgetc(stream);
    }
    
    if (count == 0) {
        if (c != EOF) {
            ungetc(c, stream);
        }
        return 0; 
    }
    
    if (c != EOF) {
        ungetc(c, stream);
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

    *output = otv;
    return 1; 
}

int baseFlag(FILE *stream, int *output, int base, bool capitalize)
{
    if (base < 2 || base > 36) {
        base = 10;
    }
    
    int c;
    long long otv = 0;
    int sign = 1;
    bool empty = true;

    while (isspace(c = fgetc(stream))) {
        if (c == EOF) {
            return 0;
        }
    }

    if (c == '-') {
        sign = -1;
        c = fgetc(stream);
    }

    while (c != EOF && !isspace(c)) {
        int value = getValue(c, capitalize);

        if (value < 0 || value >= base) {
            ungetc(c, stream);
            break;
        }

        empty = false;

        if ((otv > (2147483647 / base)) || (otv < 0)) {
            while (c != EOF && !isspace(c)) {
                c = fgetc(stream);
            }
            if (c != EOF) {
                ungetc(c, stream);
            }

            *output = 0;
            return 1;
        }
        
        otv = otv * base + value;
        c = fgetc(stream);
    }
    
    if (c != EOF) {
        ungetc(c, stream);
    }

    if (empty) {
        return 0;
    }

    *output = (int)(otv * sign);
    return 1;
}

// Функция анализа флагов

FlagType flagTyper(const char* format)
{
    const char * curr = format;
    
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

int boringFlag(FILE *stream, const char **format, va_list *args)
{

    char flag[128];    
    char *currWrite = flag;
    const char *standart = "diuoxXfFeEgGaAcspn%%";

    *currWrite++ = '%';
    
    const char *curr = *format;

    while ((currWrite - flag < 127) && *curr != '\0' && strchr(standart, *curr) == NULL) {
        *currWrite++ = *curr;
        curr++;
    }

    if (*curr != '\0') {
        *currWrite++ = *curr;
        curr++;
    }

    *currWrite = '\0';

    *format = curr;

    return vfscanf(stream, flag, *args);
}

int completeFlag(FlagType flag, FILE *stream, va_list *args)
{
    switch (flag) {
        case ROMAN:
            return romanFlag(stream, va_arg(*args, int*));
        case ZECKENDORF:
            return zeckendorfFlag(stream, va_arg(*args, unsigned int*));
        case LOWER:
        case UPPER:
            return baseFlag(stream, va_arg(*args, int*), va_arg(*args, int), (flag == UPPER));
        default:
            return -1;
    }
}

int overfscanf(FILE * stream, const char *format, ...)
{
    va_list args;
    va_start(args, format);

    const char *curr = format;
    int written = 0;

    while (*curr != '\0') 
    {
        if (*curr == '%') 
        {
            curr++;
            
            if (*curr == '%') {
                int c = fgetc(stream);
                curr++;
                continue;
            }
            
            FlagType typeOfFlag = flagTyper(curr);
            int currWritten = 0;
            
            if (typeOfFlag == OTHER) {
                // Все остальные флаги
                currWritten = boringFlag(stream, &curr, &args);

            } else {
                curr+= 2;
                // Обработка кастомного флага
                currWritten = completeFlag(typeOfFlag, stream, &args);
            }
            
            if (currWritten == 1) {
                written++;
            } else {
                va_end(args);
                return written;
            }

        } else {
            int c = fgetc(stream);
            curr++;
        }
    }

    va_end(args);
    return written;
}