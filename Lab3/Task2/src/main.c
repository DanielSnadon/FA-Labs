#include "vector.h"
#include <stdlib.h>

VECTOR_TYPE copy(VECTOR_TYPE value) {
    return value;
}

void delete(VECTOR_TYPE value) {
    value = 0;
}

int main(int argc, char* argv[]) {
    Vector testV = create_vector(4, copy, delete);

    printf("Начальная capacity - %d \n", testV.capacity);

    push_back_vector(&testV, 10);
    push_back_vector(&testV, 20);
    push_back_vector(&testV, 30);
    push_back_vector(&testV, 40);
    push_back_vector(&testV, 50);

    printf("Перевыделенная capacity - %d \n", testV.capacity);

    printf("Элемент по индексу 2: %d \n", get_at_vector(&testV, 2));

    delete_at_vector(&testV, 2);

    printf("Элемент по индексу 2: %d \n", get_at_vector(&testV, 2));

    Vector* anotherV = copy_vector_new(&testV);

    printf("Равны ли вектора -> %d (1 - да, 0 - нет) \n", is_equal_vector(&testV, anotherV));

    push_back_vector(anotherV, 50);

    printf("Равны ли вектора -> %d (1 - да, 0 - нет) \n", is_equal_vector(&testV, anotherV));
    
    delete_vector(anotherV);

    erase_vector(&testV);

    printf("Size - %d \n", testV.size);

    return 0;
}