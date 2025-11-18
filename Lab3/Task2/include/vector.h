#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>

#define VECTOR_TYPE int

typedef struct {
    VECTOR_TYPE *data;
    size_t size;
    size_t capacity;

    VECTOR_TYPE (*CopyVoidPtr) (VECTOR_TYPE);
    void (*DeleteVoidPtr) (VECTOR_TYPE);
} Vector;

Vector create_vector(size_t initial_capacity,
                    VECTOR_TYPE (*CopyFunc)(VECTOR_TYPE),
                    void (*DeleteFunc)(VECTOR_TYPE));

void erase_vector(Vector *v);

int is_equal_vector(const Vector *v1, const Vector *v2);

void copy_vector(Vector *dest, const Vector *src);

Vector *copy_vector_new(const Vector *src);

void push_back_vector(Vector *v, VECTOR_TYPE value);

void delete_at_vector(Vector *v, size_t index);

VECTOR_TYPE get_at_vector(const Vector *v, size_t index);

void delete_vector(Vector *v);

#endif