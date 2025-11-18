#ifndef HEAP_H
#define HEAP_H

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *data;
    size_t size;
    size_t capacity;
} Heap;

Heap create_heap(size_t initial_capacity);

void delete_heap(Heap *h);

int is_empty_heap(const Heap *h);

size_t size_heap(const Heap *h);

int peek_heap(const Heap *h);

void push_heap(Heap *h, int value);

int pop_heap(Heap *h);

Heap build_heap(const int *array, size_t n);

int is_equal_heap(const Heap *h1, const Heap *h2);

#endif