#include "fheap.h"

// Вспомогательные функции

void insertNode(FNode* new, FNode* dst)
{
    if (new == NULL || dst == NULL) {
        return;
    }

    new->right = dst;
    new->left = dst->left;
    dst->left->right = new;
    dst->left = new;
}

void removeNode(FNode* n)
{
    if (n == NULL) {
        return;
    }

    n->left->right = n->right;
    n->right->left = n->left;
    n->left = n;
    n->right = n;
}

FNode* createFNode(int x)
{
    FNode* new = (FNode*)malloc(sizeof(FNode));
    if (new == NULL) {
        return (FNode*)0;
    }

    new->key = x;
    new->degree = 0;
    new->parent = NULL;
    new->child = NULL;
    new->left = new;
    new->right = new;

    return new;
}

// Основные функции

FibonacciHeap createFibonacciHeap()
{
    FibonacciHeap h;
    h.min = NULL;
    h.size = 0;
    return h;
}

FNode* merge(FNode* n1, FNode* n2)
{
    if (n1 == NULL) {
        return n2;
    }
    if (n2 == NULL) {
        return n1;
    }

    FNode* n1R = n1->right;
    FNode* n2L = n2->left;

    n1->right = n2;
    n2->left = n1;

    n2L->right = n1R;
    n1R->left = n2L;

    return n1;
}

int add(FibonacciHeap *h, int x)
{
    if (h == NULL) {
        return -1;
    }

    FNode* new = createFNode(x);
    if (!new) {
        return 1;
    }

    if (h->min == NULL) {
        h->min = new;
    } else {
        merge(h->min, new);

        if (new->key < h->min->key) {
            h->min = new;
        }
    }

    h->size++;
    return 0;
}

int del(FibonacciHeap *h)
{
    

    if (h == NULL) {
        return -1;
    }
    if (h->min == NULL) {
        return 1;
    }

    FNode* minNode = h->min;
    
    if (minNode->child) {
        FNode* kid = minNode->child;
        do {
            kid->parent = NULL;
            kid = kid->right;            
        } while (kid != minNode->child);

        h->min = merge(h->min, minNode->child);
    }

    FNode* rememberToFree = minNode;
    FNode* next = minNode->right;

    if (minNode->right == minNode) {
        h->min = NULL;
    } else {
        removeNode(minNode);
        h->min = next;
        consolidate(h);
    }

    h->size--;
    free(rememberToFree);

    return 0;
}

int peek(FibonacciHeap *h)
{
    if (h == NULL) {
        return 0;
    }
    return h->min->key;
}

void freeFNode(FNode* n)
{
    if (n == NULL) {
        return;
    }

    FNode* start = n;
    FNode* current = n;

    do {
        FNode* next = current->right;

        if (current->child) {
            freeFNode(current->child);
        }

        free(current);
        current = next;
    } while (current != start);
}

void freeFHeap(FibonacciHeap *h)
{
    if (h && h->min) {
        freeFNode(h->min);
    }
    
    h->min = NULL;
    h->size = 0;
}

// Консолидация и вспомогательное соединение родителя и ребёнка

void unite(FNode* newChild, FNode* parent)
{
    removeNode(newChild);

    newChild->parent = parent;

    if (parent->child == NULL) {
        parent->child = newChild;
        newChild->left = newChild;
        newChild->right = newChild;
    } else {
        merge(parent->child, newChild);
    }

    parent->degree++;
}

void consolidate(FibonacciHeap *h)
{
    if (h->min == NULL) {
        return;
    }

    FNode* degrees[100];
    for (int i = 0; i < 100; i++) {
        degrees[i] = NULL;
    }

    FNode* start = h->min;
    FNode* current = start;

    int count = 0;
    do {
        count++;
        current = current->right;
    } while (current != start);

    FNode* roots[count];
    for (int i = 0; i < count; i++) {
        roots[i] = current;
        current = current->right;
    }

    for (int i = 0; i < count; i++) {
        FNode* x = roots[i];
        int deg = x->degree;

        while (degrees[deg] != NULL) {
            FNode* y = degrees[deg];

            FNode* parent;
            FNode* newChild;

            if (x->key < y->key) {
                parent = x;
                newChild = y;
            } else {
                parent = y;
                newChild = x;
            }

            degrees[deg] = NULL;

            unite(newChild, parent);

            x = parent;
            deg = x->degree;
        }
        
        degrees[deg] = x;
    }

    h->min = NULL;

    for (int i = 0; i < 100; i++) {
        if (degrees[i] != NULL) {
            FNode* curr = degrees[i];

            curr->left = curr;
            curr->right = curr;

            if (h->min == NULL) {
                h->min = curr;
            } else {
                merge(h->min, curr);

                if (curr->key < h->min->key) {
                    h->min = curr;
                }
            }
        }
    }
}
