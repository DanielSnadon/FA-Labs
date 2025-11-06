#include "functions.h"
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc != 1) {
        printf("Ошибка: программа не принимает аргументов. \n");
        return 1;
    }

    printf("Oversscanf (Строка)\n");
    
    // %Zr (Цекендорф) | %Ro (Римское) | %d (Десятичное)
    const char *input_str = "11011 15 XIV";
    const char *format_str = " %Zr %d %Ro";
    
    unsigned int val1 = 0;
    int val2 = 0;
    int val3 = 0;

    int scanned1 = oversscanf(input_str, format_str, &val1, &val2, &val3);

    printf("%%Zr (11011) = %u\n", val1);
    printf("%%d (15) = %d\n", val2);
    printf("%%Ro (XIV) = %d\n", val3);
    
    printf("Вывод функции - %d\n", scanned1);

    printf("\nOverfscanf (Файл)\n");
    
    const char *filename = "test.txt";
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Ошибка: Не удалось открыть файл");
        return 1;
    }

    int val4 = 0; 
    float val5 = 0;
    int val6 = 0;

    // %CV (База 36, верхний регистр) | %d | %Ro (Римское)
    const char *format_file = "%CV %f %Ro";
    
    int scanned2 = overfscanf(file, format_file, 36, &val4, &val5, &val6);

    printf(" - %%CV (10A, база 36) = %d\n", val4);
    printf(" - %%f (0.5) = %lf\n", val5);
    printf(" - %%Ro (MCMXLV) = %d\n", val6);
    printf("Вывод функции - %d\n", scanned2);
    
    fclose(file);

    return 0;
}