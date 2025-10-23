#include "another.h"

// Пункт j - макрос:
#if defined(__linux__)

    #define AMOUNT_OF_ERRORS 5
    static const char *const LINUX_ERRORS[] = {
        "Success",
        "Operation not permitted",      // 1: EPERM
        "No such file or directory",    // 2: ENOENT
        "No such process",              // 3: ESRCH
        "Interrupted system call"       // 4: EINTR
    };
    
    #define ERRORS_ARRAY LINUX_ERRORS

#elif defined(__APPLE__)

    #define AMOUNT_OF_ERRORS 5
    static const char *const MACOS_ERRORS[] = {
        "Success",
        "Not permitted",                // 1: EPERM
        "No such file",                 // 2: ENOENT
        "No process",                   // 3: ESRCH
        "Interrupted"                   // 4: EINTR
    };
    
    #define ERRORS_ARRAY MACOS_ERRORS

#else

    #define AMOUNT_OF_ERRORS 1 
    static const char *const UNKNOWN_ERRORS[] = {
        "Unknown error"
    };

    #define ERRORS_ARRAY UNKNOWN_ERRORS

#endif



// Функция поиска первого вхождения среди первых n байтов.
// Возвращает указатель на найденный символ.
// Иначе возвращает NULL.
void *memchr(const void *str, int c, size_t n) // a
{
    if (str == NULL) {
        return NULL;
    }

    const unsigned char *curr = (const unsigned char *)str;

    unsigned char subject = (unsigned char)c;

    for (size_t i = 0; i < n; i++) {

        if (*curr == subject) {
            return (void *)curr;
        }

        curr++;
    }

    return NULL;
}

// Функция сравнения байтов.
// Возвращает: 1 - str1>str2 / 0 - str1==str2 / -1 - str1<str2.
// В случае невалидных указателей возвращает 2.
int memcmp(const void *str1, const void *str2, size_t n) // b
{
    if (str1 == NULL || str2 == NULL) {
        return 2;
    }

    const unsigned char *curr1 = (const unsigned char *)str1;
    const unsigned char *curr2 = (const unsigned char *)str2;

    for (size_t i = 0; i < n; i++) {

        if (*curr1 < *curr2) {
            return -1;
        } else if (*curr1 > *curr2) {
            return 1;
        }

        curr1++;
        curr2++;
    }

    return 0;
}

// Функция копирования.
// Возвращает первый символ строки, куда копировали.
void *memcpy(void *dest, const void *src, size_t n) // c
{
    if (dest == NULL || src == NULL) {
        return NULL;
    }

    unsigned char *to = (unsigned char *)dest;
    const unsigned char *from = (const unsigned char *)src;

    for (size_t i = 0; i < n; i++) {

        *to = *from;

        to++;
        from++;
    }
    
    return dest;
}

// Функция заполнения символом.
// Возвращает первый символ преобразованной строки.
void *memset(void *str, int c, size_t n) // d
{
    if (str == NULL) {
        return NULL;
    }

    unsigned char subject = (unsigned char)c;
    unsigned char *place = (unsigned char *)str;

    for (size_t i = 0; i < n; i++) {
        *place = subject;
        place++;
    }

    return str;
}

// Функция "пришивания строки".
// Возвращает указатель на первый символ "пришитого" участка.
void *strncat(char *dest, const char *src, size_t n)  // e
{
    if (dest == NULL || src == NULL) {
        return NULL;
    }

    char* curr = dest;

    while (*curr != '\0') {
        curr++;
    }

    for (size_t i = 0; i < n; i++) {
        *curr = *src;
        curr++;
        src++;
    }

    return (void*)src;
}

// Функция поиска первого вхождения во всей строке.
// Возвращает указатель на найденный символ.
// Иначе возвращает NULL.
char *strchr(const char *str, int c) // f
{
    if (str == NULL) {
        return NULL;
    }

    unsigned char subject = (unsigned char)c;

    do {

        if (*str == subject) {
            return (char *)str;
        }

    } while (*str++ != '\0');
    
    return NULL;
}

// Функция сравнения байтов.
// Возвращает: 1 - str1>str2 / 0 - str1==str2 / -1 - str1<str2.
// В случае невалидных указателей возвращает 2.
// Также, если встречает символ '\0', то после него заканчивает сравнение.
int strncmp(const char *str1, const char *str2, size_t n) // g
{
    if (str1 == NULL || str2 == NULL) {
        return 2;
    }

    for (size_t i = 0; i < n; i++) {
        if (*str1 < *str2) {
            return -1;
        } else if (*str1 > *str2) {
            return 1;
        }        

        if (*str1 == '\0' || *str2 == '\0') {
            return 0;
        }

        str1++;
        str2++;
    }

    return 0;
}

// Функция копирования.
// Возвращает первый символ строки, куда копировали.
// Также, если встречает символ '\0', то после него заканчивает вставку.
char *strncpy(char *dest, const char *src, size_t n) // h
{
    if (dest == NULL || src == NULL) {
        return NULL;
    }

    char *firstchar = dest;

    for (size_t i = 0; i < n; i++) {
        if (*src == '\0') {
            return firstchar;
        }

        *dest = *src;
        
        dest++;
        src++;
    }
    
    return firstchar;
}

// Функция подсчёта длины сегмента без букв второй строки.
// В случае невалидных указателей возвращает 0.
size_t strcspn(const char *str1, const char *str2) // i
{
    if (str1 == NULL || str2 == NULL) {
        return 0;
    }

    const char *firstchar = str2;
    size_t otv = 0;

    while (*str1 != '\0') {
        str2 = firstchar;
        while (*str2 != '\0') {
            if (*str1 == *str2) {
                return otv;
            }
            str2++;
        }
        str1++;
        otv++;
    }

    return otv;
}

// Функция поиска текста ошибки по номеру.
// Возвращает текст ошибки.
char *strerror(int errnum) // j
{
    if (errnum >= 0 && errnum < AMOUNT_OF_ERRORS) {

        return (char *)ERRORS_ARRAY[errnum];
    } else {

        return (char *)"Unknown error"; 
    }
}

// Функция вычисления длины строки без \0.
size_t strlen(const char *str) // k
{
    if (str == NULL) {
        return 0;
    }

    size_t otv = 0;

    while (*str != '\0') {
        otv++;
        str++;
    }

    return otv;
}

// Поиск первого вхождения любого символа из второй строки.
// Возвращает указатель на найденный символ.
char *strpbrk(const char *str1, const char *str2) // l
{
    if (str1 == NULL || str2 == NULL) {
        return NULL;
    }

    const char *firstchar = str2;

    while (*str1 != '\0') {
        str2 = firstchar;
        while (*str2 != '\0') {
            if (*str1 == *str2) {
                return (char *)str1;
            }
            str2++;
        }
        str1++;
    }

    return NULL;
}

// Функция поиска последнего вхождения во всей строке.
// Возвращает указатель на найденный символ.
// Иначе возвращает NULL.
char *strrchr(const char *str, int c) // m
{
    if (str == NULL) {
        return NULL;
    }

    const char *last = NULL;

    unsigned char subject = (unsigned char)c;

    do {

        if (*str == subject) {
            last = str;
        }

    } while (*str++ != '\0');
    
    return (char *)last;
}

// Функция поиска полного вхождения строки в строку.
// Возвращает указатель на начало найденного фрагмента.
// Иначе возвращает NULL.
char *strstr(const char *haystack, const char *needle) // n
{
    if (haystack == NULL || needle == NULL) {
        return NULL;
    }

    const char *firstNeedle = needle;
    const char *otv = NULL;

    while (*haystack != '\0') {
        if (*haystack == *firstNeedle) {
            otv = haystack;

            while (*needle != '\0') {
                if (*needle != *haystack) {
                    needle = firstNeedle;
                    break;
                }

                needle++;
                haystack++;
            }

            if (*needle == '\0') {
                return (char *)otv;
            }
        }

        needle = firstNeedle;
        haystack++;
    }

    return (char *)otv;
}

// Функция разбивающая строку str на ряд токенов, разделенных delim.
// Если str != NULL, начинается поиск в новой строке.
// Если str == NULL, продолжается поиск в предыдущей строке.
char *strtok(char *str, const char *delim) // o
{
    static char *previous = NULL; 
    
    char *start;
    
    if (str != NULL) {
        previous = str; 
    } else if (previous == NULL) {
        return NULL; // Токенов больше нет...
    }
    
    char *p = previous;

    while (*p != '\0') { // Поиск начала токена
        int found = 0;
        const char *d = delim;
        
        while (*d != '\0') {
            if (*p == *d) {
                found = 1;
                break;
            }
            d++;
        }
        
        if (found) {
            p++;
        } else {
            break;
        }
    }
    
    if (*p == '\0') {
        previous = NULL;
        return NULL;
    }

    start = p;


    while (*p != '\0') { // Поиск конца токена
        int is_delimiter = 0;
        const char *d = delim;
        
        while (*d != '\0') {
            if (*p == *d) {
                is_delimiter = 1;
                break;
            }
            d++;
        }
        
        if (is_delimiter) {
            break;
        }
        p++;
    }
    
    if (*p != '\0') {
        *p = '\0'; 
        previous = p + 1;
    } else {
        previous = NULL; 
    }

    return start;
}