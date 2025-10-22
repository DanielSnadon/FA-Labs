#include "functions.h"
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc != 1) {
        printf("Ошибка: программа не принимает аргументов. \n");
        return 1;
    }

    FILE *f = fopen("out.txt", "w");
    if (f == NULL) {
        fprintf(stderr, "Ошибка: не удалось открыть файл out.txt\n");
        return 1;
    }

    char buffer[1024];
    int len;

    int testInt = 42;
    unsigned int testUInt = 1234567;
    float testFloat = 3.14f;
    double testDouble = 100.0001;
    int testBase = 16;

    // OVERFPRINTF

    // 1. %Ro
    overfprintf(f, "1.1. Римские (1999): %Ro\n", 1999);
    overfprintf(f, "1.2. Римские (24): %Ro\n", 24);

    // 2. %Zr
    overfprintf(f, "2.1. Цекендорф (13): %Zr\n", 13);
    overfprintf(f, "2.2. Цекендорф (42): %Zr\n\n", 42);

    // 3. %Cv / %CV (Из Base 10)
    overfprintf(f, "3.1. Из 10-ой (255 в 16-ричную): %Cv\n", 255, 16);
    overfprintf(f, "3.2. Из 10-ой (1002 в 36-ричную, верхний регистр): %CV\n\n", 1002, 36);

    // 4. %to / %TO (В Base 10)
    overfprintf(f, "4.1. В Base 10 (\"ff\" из 16-ричную): %to\n", "ff", 16);
    overfprintf(f, "4.2. В Base 10 (\"RU\" из 36-ричную, верхний регистр): %TO\n\n", "RU", 36);

    // 5. Дамп памяти (%mi, %mu, %md, %mf)
    overfprintf(f, "5.1. Дамп INT (42): %mi\n", testInt);
    overfprintf(f, "5.2. Дамп UINT (1234567): %mu\n", testUInt);
    overfprintf(f, "5.3. Дамп FLOAT (3.14): %mf\n", testFloat);
    overfprintf(f, "5.4. Дамп DOUBLE (100.0001): %md\n\n", testDouble);

    // OVERSPRINTF
    
    // Римские и переводы
    len = oversprintf(buffer, "6.1. %Ro (римские), в %d-ричной: %to", 200, testBase, "ca", testBase);
    printf("(Записано %d): %s\n", len, buffer);
    
    // Цекен и из 10-ой в 2-ую
    len = oversprintf(buffer, "6.2. %Zr и %CV", 40, 256, 2);
    printf("(Записано %d): %s\n", len, buffer);

    // Дамп int
    len = oversprintf(buffer, "6.3. 1234567890 - %mi", 1234567890);
    printf("(Записано %d): %s\n", len, buffer);
    
    // Стандартный флаг
    len = oversprintf(buffer, "6.4. %%.3f: 255: %.3f", 0.123456);
    printf("(Записано %d): %s\n", len, buffer);

    fclose(f);
    printf("\nПрограмма выполнена успешно. Данные записаны в 'out.txt' и консоль.\n");

    return 0;
}