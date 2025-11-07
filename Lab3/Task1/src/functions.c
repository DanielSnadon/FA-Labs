#include "functions.h"

#include <stdlib.h>
#include <string.h>

// & - И - остаток от деления на 2^r
// >> - сдвиг вправо - деление на 2
// << - сдвиг влево - увеличение разряда
// | - ИЛИ
// ^ - XOR (если разные)
// ~ - обращение

// Добавить 1
unsigned int addOne(unsigned int a) {
    unsigned int one = 1;

    while (one) {
        unsigned int carry = a & one;
        a = a ^ one;
        one = carry << 1;
    }

    return a;
}

// Вычесть 1
unsigned int subtractOne(unsigned int a) {

    return addOne(~addOne(addOne(~a)));
    // unsigned int one = 1;

    // while (one) {
    //     unsigned int carry = (~a) & one;
    //     a =  a ^ one;
    //     one = carry << 1;
    // }

    // return a;
}


ErrorCode completeTask(const unsigned int number, const int r, char *result) {
    if (result == NULL) {
        return ERROR_INVALID_POINTER;
    }
    if (r < 1 || r > 5) {
        return ERROR_INVALID_BASE;
    }

    unsigned int tempNumber = number;
    const unsigned int maxBits = 32;
    char tempResult[33];

    for (unsigned int i = 0; i < maxBits; i = addOne(i)) {
        tempResult[i] = '0';
    }
    tempResult[maxBits] = '\0';

    int index = subtractOne(maxBits);

    const unsigned int mask = subtractOne(1 << r);

    const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    unsigned int ostatok = 0;
    while (tempNumber != 0) {
        ostatok = tempNumber & mask;
        tempResult[index] = digits[ostatok]; 
        index = subtractOne(index);
        tempNumber = tempNumber >> r;
    }


    unsigned int startPos = 0;
    while (tempResult[startPos] == '0' && startPos != subtractOne(maxBits)) {
        startPos = addOne(startPos);
    }


    unsigned int newPos = 0;
    unsigned int currPos = startPos;
    
    while (tempResult[currPos] != '\0') {
        result[newPos] = tempResult[currPos];
        newPos = addOne(newPos);
        currPos = addOne(currPos);
    }
    result[newPos] = '\0';

    return SUCCESS;
}