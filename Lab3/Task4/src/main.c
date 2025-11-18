#include "heap.h"
#include <stdlib.h>

void printHeap(const Heap *h) {
    printf("[");
    for (size_t i = 0; i < h->size; i++) {
        printf("%d,", h->data[i]);
    }
    printf("]\n");
}

int main(int argc, char* argv[]) {

    Heap testH = create_heap(3);

    printf("Начальная capacity - %d \n", testH.capacity);

    push_heap(&testH, 50);
    push_heap(&testH, 10);
    push_heap(&testH, 40);
    push_heap(&testH, 20);
    push_heap(&testH, 30);

    printf("Перевыделенная capacity - %d \n", testH.capacity);

    printf("Куча -> ");
    printHeap(&testH);

    printf("Peek -> %d \n", peek_heap(&testH));

    printf("Удаление минимального \n", pop_heap(&testH));
    printf("Куча -> ");
    printHeap(&testH);

    int arr[] = {9, 12, 17, 3, 15, 20};

    Heap builtH = build_heap(arr, 6);

    printf("Куча, построенная из массива {9, 12, 17, 3, 15, 20} -> ");
    printHeap(&builtH);

    printf("Равны ли кучи? (1 - равны, 0 - не равны) -> %d \n", is_equal_heap(&testH, &builtH));

    delete_heap(&testH);
    delete_heap(&builtH);

    printf("Пуста ли куча? (1 - да, 0 - нет) -> %d \n", is_empty_heap(&testH));

    return 0;
}