#include "vector.h"

Vector create_vector(size_t initial_capacity,
                    VECTOR_TYPE (*CopyFunc)(VECTOR_TYPE),
                    void (*DeleteFunc)(VECTOR_TYPE))
{
    Vector v;

    v.size = 0;
    v.capacity = initial_capacity;
    v.CopyVoidPtr = CopyFunc;
    v.DeleteVoidPtr = DeleteFunc;

    if (initial_capacity > 0) {

        v.data = (VECTOR_TYPE*)malloc(v.capacity * sizeof(VECTOR_TYPE));

        if (v.data == NULL) {
            printf("Ошибка: не удалось выделить память. Вектор инициализрован с нулевой вместимостью. \n");
            v.capacity = 0;
        }
    } else {

        v.data = NULL;
    }

    return v;
}

void erase_vector(Vector *v)
{
    if (v == NULL) {
        return;
    }

    if (v->DeleteVoidPtr != NULL) {

        for (size_t i = 0; i < v->size; i++) {
            v->DeleteVoidPtr(v->data[i]);
        }
    }

    if (v->data != NULL) {
        free(v->data);
        v->data = NULL;
    }

    v->size = 0;
    v->capacity = 0;
}

int is_equal_vector(const Vector *v1, const Vector *v2)
{
    if (v1 == NULL || v2 == NULL) {
        if (v1 == v2) {
            return 1;
        }
        return 0;
    }

    if (v1->size != v2->size) {
        return 0;
    }

    for (size_t i = 0; i < v1->size; i++) {

        if (v1->data[i] != v2->data[i]) {
            return 0;
        }
    }

    return 1;
}

void copy_vector(Vector *dest, const Vector *src)
{
    if (dest == NULL || src == NULL || dest == src) {
        return;
    }
    
    erase_vector(dest);

    if (src->size == 0) {
        return;
    }

    if (dest->capacity < src->size) {
        
        VECTOR_TYPE *tempData = (VECTOR_TYPE*)malloc(src->size * sizeof(VECTOR_TYPE));

        if (tempData == NULL) {
            printf("Ошибка: не удалось выделить память. Вектор, в который производится копирование, теперь пуст. \n");
            return;
        }

        dest->data = tempData;
        dest->capacity = src->size;
        dest->size = src->size;

        for (size_t i = 0; i < src->size; i++) {
            
            if (dest->CopyVoidPtr != NULL) {
                dest->data[i] = dest->CopyVoidPtr(src->data[i]);
            } else {
                dest->data[i] = src->data[i];
            }
        }
    }
}

Vector *copy_vector_new(const Vector *src)
{
    if (src == NULL) {
        return NULL;
    }

    Vector *v = (Vector*)malloc(sizeof(Vector));

    if (v == NULL) {
        printf("Ошибка: не удалось выделить память для новой копии вектора. \n");
        return NULL;
    }

    v->size = src->size;
    v->capacity = src->size;
    v->CopyVoidPtr = src->CopyVoidPtr;
    v->DeleteVoidPtr = src->DeleteVoidPtr;

    if (src->size != 0) {

        v->data = (VECTOR_TYPE*)malloc(src->size * sizeof(VECTOR_TYPE));

        if (v->data == NULL) {
            printf("Ошибка: не удалось выделить память для новой копии вектора. \n");
            free(v);
            return NULL;
        }

        for (size_t i = 0; i < src->size; i++) {
            
            if (v->CopyVoidPtr != NULL) {
                v->data[i] = v->CopyVoidPtr(src->data[i]);
            } else {
                v->data[i] = src->data[i];
            }
        }
    } else {
        v->data = NULL;
    }

    return v;
}

void push_back_vector(Vector *v, VECTOR_TYPE value)
{
    if (v == NULL) {
        return;
    }

    if (v->size >= v->capacity) {

        size_t newCapacity = (v->capacity == 0) ? 1 : v->capacity * 2;

        VECTOR_TYPE *newData = (VECTOR_TYPE*)realloc(v->data, newCapacity * sizeof(VECTOR_TYPE));

        if (newData == NULL) {
            printf("Ошибка: не удалось перевыделить память при добавлении нового элемента. \n");
            return;
        }

        v->data = newData;
        v->capacity = newCapacity;
    }

    if (v->CopyVoidPtr != NULL) {
        v->data[v->size] = v->CopyVoidPtr(value);
    } else {
        v->data[v->size] = value;
    }

    v->size++;
}

void delete_at_vector(Vector *v, size_t index)
{
    if (v == NULL || index >= v->size) {
        return;
    }

    if (v->DeleteVoidPtr != NULL) {
        v->DeleteVoidPtr(v->data[index]);
    }

    for (size_t i = index; i < v->size - 1; i++) {
        v->data[i] = v->data[i + 1];
    }

    v->size--;
}

VECTOR_TYPE get_at_vector(const Vector *v, size_t index)
{
    if (v == NULL || index >= v->size) {
        printf("Ошибка: попытка получить значение из NULL-вектора или введённый индекс не является валидным для данного вектора. \n");
        return (VECTOR_TYPE)0;
    }

    return v->data[index];
}

void delete_vector(Vector *v)
{
    if (v == NULL) {
        return;
    }

    erase_vector(v);

    free(v);
}