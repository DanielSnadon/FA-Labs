#include "heap.h"

// Вспомогательные функции:

size_t parentIndex(size_t i)
{
    return (i - 1) / 2;
}

size_t leftChildIndex(size_t i)
{
    return 2 * i + 1;
}

size_t rightChildIndex(size_t i)
{
    return 2 * i + 2;
}

void swap(Heap *h, size_t i, size_t j)
{
    int temp = h->data[i];
    h->data[i] = h->data[j];
    h->data[j] = temp;
}

void heapifyUp(Heap *h, size_t i)
{
    while (i > 0 && h->data[parentIndex(i)] > h->data[i]) {

        swap(h, i, parentIndex(i));
        i = parentIndex(i);
    }
}

void heapifyDown(Heap *h, size_t i)
{
    size_t minIndex = i;
    size_t l = leftChildIndex(i);
    size_t r = rightChildIndex(i);

    if (l < h->size && h->data[l] < h->data[minIndex]) {
        minIndex = l;
    }

    if (r < h->size && h->data[r] < h->data[minIndex]) {
        minIndex = r;
    }

    if (minIndex != i) {
        swap(h, i, minIndex);
        heapifyDown(h, minIndex);
    }
}

// Основные функции:

Heap create_heap(size_t initial_capacity)
{
    Heap h;
    h.size = 0;

    if (initial_capacity == 0) {
        h.data = NULL;
        h.capacity = 0;
        
    } else {

        h.data = (int*)malloc(initial_capacity * sizeof(int));

        if (h.data == NULL) {
            printf("Ошибка: не удалось выделить необходимую память для создания кучи. Инициализирована куча с нулевой вместимостью. \n");
            h.capacity = 0;
        } else {
            h.capacity = initial_capacity;
        }
    }

    return h;
}

void delete_heap(Heap *h)
{
    if (h == NULL) {
        return;
    }

    if (h->data != NULL) {
        free(h->data);
    }

    h->size = 0;
    h->capacity = 0;
}

int is_empty_heap(const Heap *h)
{
    if (h == NULL || h->size == 0) {
        return 1;
    }
    return 0;
}

size_t size_heap(const Heap *h)
{
    if (h == NULL) {
        return 0;
    }

    return h->size;
}

int peek_heap(const Heap *h)
{
    if (is_empty_heap(h)) {
        printf("Ошибка: невозможно получить минимальный элемент, так как куча пуста. \n");
        return 0;
    }

    return h->data[0];
}

void push_heap(Heap *h, int value)
{
    if (h == NULL) {
        return;
    }

    if (h->size >= h->capacity) {

        size_t newCapacity = (h->capacity == 0) ? 1 : h->capacity * 2;

        int *newData = (int*)realloc(h->data, newCapacity * sizeof(int));

        if (newData == NULL) {
            printf("Ошибка: не удалось перевыделить память при добавлении нового элемента. \n");
            return;
        }

        h->data = newData;
        h->capacity = newCapacity;
    }

    h->data[h->size] = value;
    h->size++;

    heapifyUp(h, h->size - 1);
}

int pop_heap(Heap *h)
{
    if (is_empty_heap(h)) {
        printf("Ошибка: невозможно удалить элемент, так как куча пуста. \n");
        return 0;
    }

    int temp = h->data[0];

    h->size--;
    h->data[0] = h->data[h->size];

    if (h->size > 0) {
        heapifyDown(h, 0);
    }

    return temp;
}

Heap build_heap(const int *array, size_t n)
{
    Heap h = create_heap(n);

    if (h.capacity == 0) {
        printf("Ошибка: не удалось выделить память для кучи. \n");
        return h;
    }

    for (size_t i = 0; i < n; i++) {
        h.data[i] = array[i];
    }
    h.size = n;

    for (int j = (n / 2) - 1; j >= 0; j--) {
        heapifyDown(&h, (size_t)j);
    }

    return h;
}

int is_equal_heap(const Heap *h1, const Heap *h2)
{
    if (h1 == NULL || h2 == NULL) {
        if (h1 == h2) {
            return 1;
        }
        return 0;
    }

    if (h1->size != h2->size) {
        return 0;
    }

    for (size_t i = 0; i < h1->size; i++) {

        if (h1->data[i] != h2->data[i]) {
            return 0;
        }
    }

    return 1;
}