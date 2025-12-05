#include "fheap.h"
#include <stdlib.h>

int main(int argc, char* argv[]) {
    FibonacciHeap fibHeap = createFibonacciHeap();

    add(&fibHeap, 10);
    add(&fibHeap, 5);
    add(&fibHeap, 20);
    add(&fibHeap, 1);
    add(&fibHeap, 15);

    printf("Количество узлов - %d \n", fibHeap.size);

    printf("Минимальный элемент - %d \n", peek(&fibHeap));

    while (fibHeap.size > 0) {
        int min = peek(&fibHeap);
        del(&fibHeap);
        printf("Извлечён элемент - %d \n", min);
    }

    printf("Количество узлов - %d \n", fibHeap.size);

    add(&fibHeap, 40);
    add(&fibHeap, 5);

    printf("Количество узлов - %d \n", fibHeap.size);
    printf("Минимальный элемент - %d \n", peek(&fibHeap));

    freeFHeap(&fibHeap);

    printf("Количество узлов - %d \n", fibHeap.size);
    
    return 0;
}