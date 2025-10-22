#include "functions.h"
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

// Тип, хранящий тип флага

typedef enum {
    OTHER,
    ROMAN,             // %Ro
    ZECKENDORF,      // %Zr
    FROM_10_LOWER,   // %Cv
    FROM_10_UPPER,   // %CV
    TO_10_LOWER,     // %to
    TO_10_UPPER,     // %TO
    DUMP_INT,        // %mi
    DUMP_UINT,       // %mu
    DUMP_DOUBLE,     // %md
    DUMP_FLOAT       // %mf
} FlagType;

// Просто вспомогательные функции

int bitilize(unsigned char byte, char *output)
{
    for (int i = 7; i >= 0; i--)
    {
        if (byte & (1 << i))
        {
            *output++ = '1';
        } else
        {
            *output++ = '0';
        }
    }
    *output = '\0';
    return 8;
}

bool isValidDigit(char c, int base, bool capital) {
    if (c >= '0' && c <= '9') {
        return (c - '0') < base;
    }
    
    if (!capital && c >= 'a' && c <= 'z') {
        return (c - 'a' + 10) < base;
    }
    
    if (capital && c >= 'A' && c <= 'Z') {
        return (c - 'A' + 10) < base;
    }
    
    return false;
}

int getValue(char c) {
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

// Функции анализа флагов

FlagType flagTyper(const char* format) {
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
            return FROM_10_LOWER;
        if (curr[1] == 'V') 
            return FROM_10_UPPER;
    }

    if (curr[0] == 't' && curr[1] == 'o') {
        return TO_10_LOWER;
    }

    if (curr[0] == 'T' && curr[1] == 'O') {
        return TO_10_UPPER;
    }

    if (curr[0] == 'm') {
        if (curr[1] == 'i') return DUMP_INT;
        if (curr[1] == 'u') return DUMP_UINT;
        if (curr[1] == 'd') return DUMP_DOUBLE;
        if (curr[1] == 'f') return DUMP_FLOAT;
    }

    return OTHER;
}

int getFlagLength(FlagType flag) {
    switch (flag) {
        case ROMAN:
        case ZECKENDORF:
        case FROM_10_LOWER:
        case FROM_10_UPPER:
        case TO_10_LOWER:
        case TO_10_UPPER:
        case DUMP_INT:
        case DUMP_UINT:
        case DUMP_DOUBLE:
        case DUMP_FLOAT:
            return 2;
        default:
            return -1;
    }
}

// Функции флагов и их обёртки

int to10Flag(const char *number, int base, char *output, bool capitalize) {
    if (base < 2 || base > 36) {
        base = 10;
    }

    int otv = 0;
    int sign = 1;
    const char *curr = number;

    if (*curr == '-') {
        sign = -1;
        curr++;
    }
    
    while (*curr != '\0') {
        char c = *curr;
        int value;

        if (capitalize) {
            value = getValue(toupper(c));
        } else {
            value = getValue(tolower(c));
        }

        if (value < 0 || value >= base) {
            otv = 0;
            return sprintf(output, "%d", 0);
        }

        if ((otv > 0 && otv > (2147483647 - value) / base) || (otv < 0)) {
            otv = 0;
            return sprintf(output, "%d", 0);
        }

        otv = otv * base + value;
        curr++;
    }
    
    return sprintf(output, "%d", otv * sign);
}

int prelower_to10Flag(va_list args, char *output) { // Для %to
    char *str = va_arg(args, char *);
    int base = va_arg(args, int);
    
    return to10Flag(str, base, output, false);
}

int preupper_to10Flag(va_list args, char *output) { // Для %TO
    char *str = va_arg(args, char *);
    int base = va_arg(args, int);

    return to10Flag(str, base, output, true);
}

int from10Flag(int base, int number, char *output, bool capitalize)
{
    if (base < 2 || base > 36) {
        base = 10;
    }
    
    if (base == 10) {
        return sprintf(output, "%d", number);
    }
    
    if (number == 0) {
        output[0] = '0';
        output[1] = '\0';
        return 1;
    }
    
    char buff[34];
    int i = 0;
    bool negativness = false;
    
    if (number < 0) {
        number = -number;
        negativness = true;
    }
    
    char letterType = capitalize ? 'A' : 'a';
    
    while (number > 0) {
        int digit = number % base;
        
        if (digit < 10) {
            buff[i] = digit + '0';
        } else {
            buff[i] = (digit - 10) + letterType;
        }
        
        number /= base;
        i++;
    }

    if (negativness) {
        buff[i++] = '-';
    }
    
    char *currWrite = output;

    for (int k = i - 1; k >= 0; k--) {
        *currWrite++ = buff[k];
    }
    
    *currWrite = '\0';

    return currWrite - output;
}

int prelower_from10Flag(va_list args, char *output) // Для %Cv
{
    int number = va_arg(args, int);
    int base = va_arg(args, int);
    
    return from10Flag(base, number, output, false);
}

int preupper_from10Flag(va_list args, char *output) // Для %CV
{
    int number = va_arg(args, int);
    int base = va_arg(args, int);

    return from10Flag(base, number, output, true);
}

// Для %Zr

int zeckendorfFlag(unsigned int number, char *output)
{
    if (number == 0) {
        strcpy(output, "01");
        return 2; 
    }
    
    unsigned int fibi[47]; // Больше 47 == Больше unsigned int'а
    fibi[0] = 1;
    fibi[1] = 2;
    int k = 1;

    for (int i = 2; i < 47; i++) {
        unsigned int next = fibi[i-1] + fibi[i-2];
        
        if (next < fibi[i-1] || next > number) {
            k = i - 1;
            break;
        }

        fibi[i] = next;
        k = i;
    }
    
    char otv[48];
    int len = 0;
    unsigned int temp = number;
    
    for (int i = k; i >= 0; i--) {
        if (temp >= fibi[i]) {
            otv[len++] = '1';
            temp -= fibi[i];
        } else {
            otv[len++] = '0';
        }
    }

    int outInd = 0;
    for (int i = 0; i < len; i++) {
        output[outInd++] = otv[i];
    }

    output[outInd++] = '1';
    output[outInd] = '\0';

    return outInd;
}

int preZeckendorfFlag(va_list args, char *output)
{

    unsigned int number = va_arg(args, unsigned int);
    
    return zeckendorfFlag(number, output);
}

// Для %Ro

int romanFlag(int number, char *output)
{
    if (number <= 0 || number > 3999) {
        int len = sprintf(output, "[%d]", number);
        
        if (len < 0) {
            return 0;
        } else {
            return len;
        }
    }

    const char *rimskie[] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
    const int values[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};

    char *currWrite = output;
    
    for (int i = 0; i < 13; i++) {

        while (number >= values[i]) {

            size_t len = strlen(rimskie[i]);
            strncpy(currWrite, rimskie[i], len);
            currWrite += len;

            number -= values[i];
        }
    }

    *currWrite = '\0';
    return currWrite - output;
}

int preRomanFlag(va_list args, char *output)
{
    int number = va_arg(args, int);
    
    return romanFlag(number, output);
}

int dumpFlag(void *number, size_t typeSize, char *output)
{
    unsigned char *bytes = (unsigned char *)number;
    char *currWrite = output;
    char buff[9];
    
    for (size_t i = 0; i < typeSize; i++)
    {
        bitilize(bytes[i], buff);
        
        strncpy(currWrite, buff, 8);
        currWrite += 8;
        
        if (i < typeSize - 1)
        {
            *currWrite++ = ' ';
        }
    }

    *currWrite = '\0';
    return currWrite - output;
}

int preDumpIntFlag(va_list args, char *output) { // Для %mi
    int number = va_arg(args, int); 
    return dumpFlag(&number, sizeof(int), output);
}

int preDumpUIntFlag(va_list args, char *output) { // Для %mu
    unsigned int number = va_arg(args, unsigned int);
    return dumpFlag(&number, sizeof(unsigned int), output);
}

int preDumpDoubleFlag(va_list args, char *output) { // Для %md
    double number = va_arg(args, double);
    return dumpFlag(&number, sizeof(double), output);
}

int preDumpFloatFlag(va_list args, char *output) { // Для %mf
    double temp = va_arg(args, double);
    float number = (float)temp;
    return dumpFlag(&number, sizeof(float), output);
}

// Для нормальных флагов

int boringFlag(const char **format, va_list args, char *output)
{
    char flag[128];
    char *currWrite = flag;
    const char *standart = "diuoxXfFeEgGaAcspn%%";

    *currWrite++ = '%';
    
    const char *curr = *format;
    
    while ((currWrite - flag < 127) && *curr != '\0' && strchr(standart, *curr) == NULL)
    {
        *currWrite++ = *curr;
        curr++;
    }

    if (*curr != '\0')
    {
        *currWrite++ = *curr;
        curr++;
    }

    *currWrite = '\0';

    *format = curr;

    return vsprintf(output, flag, args);
}

// Основная распределяющая функция

int completeFlag(FlagType flag, va_list args, char *output)
{
    switch (flag) {
        case ROMAN:
            return preRomanFlag(args, output);
        case ZECKENDORF:
            return preZeckendorfFlag(args, output);
        case FROM_10_LOWER:
            return prelower_from10Flag(args, output);
        case FROM_10_UPPER:
            return preupper_from10Flag(args, output);
        case TO_10_LOWER:
            return prelower_to10Flag(args, output);
        case TO_10_UPPER:
            return preupper_to10Flag(args, output);
        case DUMP_INT:
            return preDumpIntFlag(args, output);
        case DUMP_UINT:
            return preDumpUIntFlag(args, output);
        case DUMP_DOUBLE:
            return preDumpDoubleFlag(args, output);
        case DUMP_FLOAT:
            return preDumpFloatFlag(args, output);
        default:
            return -1;
    }
}

// Сами функции

int overfprintf(FILE * stream, const char *format, ...)
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
                fputc(*curr, stream);
                written++;
                curr++;
                continue;
            }

            char bucket[1024];
            
            FlagType typeOfFlag = flagTyper(curr);
            int currWritten = 0;
            
            if (typeOfFlag == OTHER) {
                // Все остальные флаги
                currWritten = boringFlag(&curr, args, bucket);

            } else {
                curr+= 2;
                // Вот оно, обработка custom флага...
                currWritten = completeFlag(typeOfFlag, args, bucket);
            }
            
            if (currWritten < 0) {
                va_end(args);
                return -1;
            }
            
            bucket[currWritten] = '\0';

            fprintf(stream, "%s", bucket);
            written += currWritten;

        } else {
            fputc(*curr, stream);
            written++;
            curr++;
        }
    }

    va_end(args);

    return written;
}

int oversprintf(char *str, const char *format, ...)
{
    va_list args;
    va_start(args, format);

    const char *curr = format;
    char *currWrite = str;

    while (*curr != '\0') 
    {
        if (*curr == '%') 
        {
            curr++;
            
            if (*curr == '%') {
                *currWrite++ = *curr;
                curr++;
                continue;
            }

            char bucket[1024];
            
            FlagType typeOfFlag = flagTyper(curr);
            int currWritten = 0;
            
            if (typeOfFlag == OTHER) {
                // Все остальные флаги
                currWritten = boringFlag(&curr, args, bucket);

            } else {
                curr+= 2;
                // Вот оно, обработка custom флага...
                currWritten = completeFlag(typeOfFlag, args, bucket);
            }
            
            if (currWritten < 0) {
                va_end(args);
                *currWrite = '\0';
                return -1;
            }
            
            strncpy(currWrite, bucket, currWritten); 
            currWrite += currWritten;

        } else {
            *currWrite++ = *curr;
            curr++;
        }
    }

    *currWrite = '\0';
    va_end(args);

    return currWrite - str;
}