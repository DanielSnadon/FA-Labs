#include "linked_list.h"

// ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ

Node* createNode(LIST_TYPE value, Node *prev, Node *next)
{
    Node *newNode = (Node*)malloc(sizeof(Node));

    if (newNode == NULL) {
        printf("Ошибка: не удалось выделить память для нового узла. \n");
        return NULL;
    }

    newNode->data = value;
    newNode->prev = prev;
    newNode->next = next;

    return newNode;
}

Node* findNode(LinkedList *list, size_t index)
{
    if (list == NULL || index >= list->size) {
        return NULL;
    }

    Node *curr = NULL;

    if (index < list->size / 2) {
        curr = list->head;
        
        for (size_t i = 0; i < index; i++) {
            curr = curr->next;
        }

    } else {
        curr = list->tail;

        for (size_t i = list->size - 1; i > index; i--) {
            curr = curr->prev;
        }
    }

    return curr;
}

int liverComparator(Liver *l1, Liver *l2)
{
    if ((strcmp(l1->name, l2->name) != 0)
        || (strcmp(l1->secondName, l2->secondName) != 0)
        || (strcmp(l1->thirdName, l2->thirdName) != 0)
        || (l1->id != l2->id)
        || (l1->birthDate.day != l2->birthDate.day)
        || (l1->birthDate.month != l2->birthDate.month)
        || (l1->birthDate.year != l2->birthDate.year)
        || (l1->income != l2->income)) {
        return 0;
    }
    return 1;
}

// БАЗОВЫЕ ОПЕРАЦИИ

LinkedList create_list(void)
{
    LinkedList l;
    l.head = NULL;
    l.tail = NULL;
    l.size = 0;
    return l;
}

void erase_list(LinkedList *list)
{
    if (list == NULL) {
        return;
    }

    Node *curr = list->head;
    Node *next = NULL;

    while (curr != NULL) {
        next = curr->next;
        free(curr);
        curr = next;
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void delete_list(LinkedList *list)
{
    if (list != NULL) {
        erase_list(list);
    }
}

void push_back_list(LinkedList *list, LIST_TYPE value)
{
    if (list == NULL) {
        return;
    }

    Node *newNode = createNode(value, list->tail, NULL);

    if (list->size == 0) {
        list->head = newNode;
        list->tail = newNode;
    } else {
        list->tail->next = newNode;
        list->tail = newNode;
    }

    list->size++;
}

void push_front_list(LinkedList *list, LIST_TYPE value)
{
    if (list == NULL) {
        return;
    }

    Node *newNode = createNode(value, NULL, list->head);

    if (list->size == 0) {
        list->head = newNode;
        list->tail = newNode;
    } else {
        list->head->prev = newNode;
        list->head = newNode;
    }

    list->size++;
}

LIST_TYPE pop_back_list(LinkedList *list)
{
    if (list == NULL || list->size == 0) {
        printf("Ошибка: не удалось получить значение удаляемого элемента. \n");
        return (LIST_TYPE){0};
    }

    Node *dying = list->tail;
    LIST_TYPE value = dying->data;

    if (list->size == 1) {
        list->head = NULL;
        list->tail = NULL;
    } else {
        list->tail = dying->prev;
        list->tail->next = NULL;
    }

    free(dying);
    list->size--;

    return value;
}

LIST_TYPE pop_front_list(LinkedList *list)
{
    if (list == NULL || list->size == 0) {
        printf("Ошибка: не удалось получить значение удаляемого элемента. \n");
        return (LIST_TYPE){0};
    }

    Node *dying = list->head;
    LIST_TYPE value = dying->data;

    if (list->size == 1) {
        list->head = NULL;
        list->tail = NULL;
    } else {
        list->head = dying->next;
        list->head->prev = NULL;
    }

    free(dying);
    list->size--;

    return value;
}

void insert_at_list(LinkedList *list, size_t index, LIST_TYPE value)
{
    if (list == NULL || index > list->size) {
        return;
    }

    if (index == 0) {
        push_front_list(list, value);
        return;
    }

    if (index == list->size) {
        push_back_list(list, value);
        return;
    }

    Node *nextNode = findNode(list, index);
    if (nextNode == NULL) {
        return;
    }

    Node *prevNode = nextNode->prev;

    Node *newNode = createNode(value, prevNode, nextNode);

    prevNode->next = newNode;
    nextNode->prev = newNode;

    list->size++;
}

void delete_at_list(LinkedList *list, size_t index)
{
    if (list == NULL || index >= list->size) {
        return;
    }

    if (index == 0) {
        pop_front_list(list);
    }

    if (index == list->size - 1) {
        pop_back_list(list);
    }

    Node *dying = findNode(list, index);
    if (dying == NULL) {
        return;
    }

    dying->prev->next = dying->next;
    dying->next->prev = dying->prev;

    free(dying);
    
    list->size--;
}

LIST_TYPE get_at_list(const LinkedList *list, size_t index)
{
    if (list == NULL) {
        printf("Ошибка: невалидное значение списка. \n");
        return (LIST_TYPE){0};
    }

    Node *search = findNode(list, index);
    if (search == NULL) {
        printf("Ошибка: не удалось получить значение элемента. \n");
        return (LIST_TYPE){0};
    }

    return search->data;
}

int is_equal_list(const LinkedList *l1, const LinkedList *l2)
{
    if (l1 == NULL || l2 == NULL) {
        return -1;
    }

    if (l1->size != l2->size) {
        return 0;
    }

    Node *check1 = l1->head;
    Node *check2 = l2->head;

    while (check1 != NULL) {
        if (liverComparator(&(check1->data), &(check2->data)) == 0) {
            return 0;
        }

        check1 = check1->next;
        check2 = check2->next;
    }
    
    return 1;
}

// СТЕК

void push_stack(LinkedList *stack, LIST_TYPE value)
{
    push_back_list(stack, value);
}

LIST_TYPE pop_stack(LinkedList *stack)
{
    return pop_back_list(stack);
}

LIST_TYPE peek_stack(const LinkedList *stack)
{
    if (stack == NULL || stack->size == 0) {
        printf("Ошибка: не удалось получить значение элемента. \n");
        return (LIST_TYPE){0};
    }

    return stack->tail->data;
}

// ОЧЕРЕДЬ

void enqueue(LinkedList *queue, LIST_TYPE value)
{
    push_back_list(queue, value);
}

LIST_TYPE dequeue(LinkedList *queue)
{
    return pop_front_list(queue);
}

LIST_TYPE peek_queue(const LinkedList *queue) {
    if (queue == NULL || queue->size == 0) {
        printf("Ошибка: не удалось получить значение элемента. \n");
        return (LIST_TYPE){0};
    }

    return queue->head->data;
}