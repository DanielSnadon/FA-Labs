#include "another.h"

int main(int argc, char* argv[]) {
    if (argc != 1) {
        printf("Ошибка: программа не принимает аргументов. \n");
        return 1;
    }

    // Переменные для тестов:

    const char test1[] = "Hello!";
    const char test2[] = "Helicopter";
    const char test3[] = "why";
    char test4[] = "WWWWWWWWWW";
    char test5[100] = "Look: ";
    const char test6[] = "I'm a \0liar.";
    const char test7[] = "I'm a \0good person.";
    char test8[] = "what/where/when";
    char twst9[] = "";

    // Тесты функций:

    printf("--- Функция memchr --- \n");

    printf("test1, test2 - %s, %s \n", test1, test2);

    printf("%c \n", *(char *)memchr(test1, 'e', 6));

    printf("%p \n", memchr(test2, 'W', 10));

    printf("--- Функция memcmp --- \n");

    printf("test1, test2, test3 - %s, %s %s \n", test1, test2, test3);

    printf("%d \n", memcmp(test1, test2, 6));

    printf("%d \n", memcmp(test1, test2, 3));

    printf("%d \n", memcmp(test1, test3, 3));

    printf("--- Функция memcpy --- \n");

    printf("test1, test3, test4 - %s, %s, %s \n", test1, test3, test4);

    memcpy(test4, test3, 3);
    printf("%s \n", test4);

    memcpy(test4, test1, 2);
    printf("%s \n", test4);

    printf("--- Функция memset --- \n");

    printf("test4, test5 - %s, %s \n", test4, test5);

    memset(test4, 'a', 5);
    printf("%s \n", test4);

    printf("--- Функция strncat --- \n");
    
    printf("test2, test5 - %s, %s \n", test2, test5);

    strncat(test5, test2, 10);
    printf("%s \n", test5);

    printf("--- Функция strchr --- \n");

    printf("test1, test5 - %s, %s \n", test1, test5);

    printf("%p \n", strchr(test1, 'l'));

    printf("%p \n", strchr(test5, 'O'));

    printf("--- Функция strncmp --- \n");

    printf("test1, test2, test3, test6, test7 - %s, %s, %s, %s, %s \n", test1, test2, test3, test6, test7);

    printf("%d \n", strncmp(test1, test2, 6));

    printf("%d \n", strncmp(test1, test2, 3));

    printf("%d \n", strncmp(test1, test3, 3));

    printf("%d \n", strncmp(test6, test7, 10));

    printf("--- Функция strncpy --- \n");

    printf("test3, test5, test6 - %s, %s, %s \n", test3, test5, test6);

    strncpy(test5, test6, 12);
    printf("%s \n", test5);

    memcpy(test5, test3, 3);
    printf("%s \n", test5);

    printf("--- Функция strcspn --- \n");

    printf("test2, test3, test4, test5 - %s, %s, %s, %s \n", test2, test3, test4, test5);

    printf("%zu \n", strcspn(test5, test4));

    printf("%zu \n", strcspn(test2, test3));

    printf("--- Функция strerror --- \n");

    printf("%s \n", strerror(0)); 
    
    printf("%s \n", strerror(2)); 
    
    printf("%s \n", strerror(999));

    printf("--- Функция strlen --- \n");

    printf("test2, test5 - %s, %s \n", test2, test5);

    printf("%zu \n", strlen(test2)); 
    
    printf("%zu \n", strlen(test5));

    printf("--- Функция strpbrk --- \n");

    printf("test1, test2, test3, test5 - %s, %s, %s, %s \n", test1, test2, test3, test5);

    printf("%c \n", *strpbrk(test5, test1));

    printf("%p \n", strpbrk(test2, test3));

    printf("--- Функция strrchr --- \n");

    printf("test1, test5 - %s, %s \n", test1, test5);

    printf("%p \n", strrchr(test1, 'l'));

    printf("%p \n", strrchr(test5, 'O'));

    printf("--- Функция strstr --- \n");

    printf("test2, test3, test5 - %s, %s \n", test2, test3, test5);

    printf("%c \n", *strstr(test5, test3));
    printf("%p \n", strstr(test5, test3));

    printf("%p \n", strstr(test2, test3));

    printf("--- Функция strtok --- \n");

    printf("test8 - %s \n", test8);

    char *token;
    const char *razdelenie = "/";
    
    printf("Token 1: %s \n", strtok("", razdelenie)); 
    
    printf("Token 2: %s \n", strtok(NULL, razdelenie)); 
    
    printf("Token 3: %s \n", strtok(NULL, razdelenie)); 

    printf("Token 4: %s \n", strtok(NULL, razdelenie)); 

    return 0;
}