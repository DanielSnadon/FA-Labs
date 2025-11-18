#include "vector.h"

Vector create_vector(size_t initial_capacity,
                    VECTOR_TYPE (*CopyFunc)(VECTOR_TYPE),
                    void (*DeleteFunc)(VECTOR_TYPE))
{
    Vector v;
    v.size = 0;
    v.capacity = initial_capacity;

    if (initial_capacity > 0) {

        v.data = (VECTOR_TYPE*)malloc(v.capacity * sizeof(VECTOR_TYPE));

        if (v.data == NULL) {
            printf("Ошибка: не удалось выделить память. Вектор инициализрован с нулевой вместимостью. \n");
            v.capacity = 0;
        }
    } else {

        v.data = NULL;
    }

    v.CopyVoidPtr = CopyFunc;
    v.DeleteVoidPtr = DeleteFunc;

    return v;
}