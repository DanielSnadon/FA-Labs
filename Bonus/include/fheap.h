#ifndef FHEAP_H
#define FHEAP_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct FNode {
    int key;
    int degree;
    struct FNode* parent;
    struct FNode* child;
    struct FNode* left;
    struct FNode* right;
} FNode;

typedef struct {
    int size;
    FNode* min;
} FibonacciHeap;

FNode* createFNode(int x);

FibonacciHeap createFibonacciHeap();

FNode* merge(FNode* n1, FNode* n2);

int add(FibonacciHeap *h, int x);

int pop(FibonacciHeap *h);

int peek(FibonacciHeap *h);

void freeFNode(FNode* n);

void freeFHeap(FibonacciHeap *h);

void consolidate(FibonacciHeap *h);

#endif
