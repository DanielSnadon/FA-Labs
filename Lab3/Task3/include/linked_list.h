#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdio.h>
#include <stdlib.h>

#define LIST_TYPE Liver

// опр. LIVER

typedef struct {
    int day;
    int month;
    int year;
} Date;

typedef struct {
    unsigned int id;
    char name[50];
    char secondName[50];
    char thirdName[50];
    Date birthDate;
    char gender;
    double income;
} Liver;

// опр. ДВУСВЯЗНЫЙ СПИСОК

typedef struct Node {
    LIST_TYPE data;
    struct Node *prev;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
    Node *tail;
    size_t size;
} LinkedList;

// опр. HISTORY

typedef enum {
    HISTORY_ADD,
    HISTORY_DELETE,
    HISTORY_MODIFY
} ActionType;

typedef struct {
    ActionType type;
    Liver oldData;
    Liver newData;
} Action;

typedef struct {
    Action actions[1000];
    int top;
    int amountOfData;
} History;

// БАЗОВЫЕ ОПЕРАЦИИ

LinkedList create_list(void);

void erase_list(LinkedList *list);

void delete_list(LinkedList *list);

void push_back_list(LinkedList *list, LIST_TYPE value);

void push_front_list(LinkedList *list, LIST_TYPE value);

LIST_TYPE pop_back_list(LinkedList *list);

LIST_TYPE pop_front_list(LinkedList *list);

void insert_at_list(LinkedList *list, size_t index, LIST_TYPE value);

void delete_at_list(LinkedList *list, size_t index);

LIST_TYPE get_at_list(const LinkedList *list, size_t index);

int is_equal_list(const LinkedList *l1, const LinkedList *l2);

// СТЕК

void push_stack(LinkedList *stack, LIST_TYPE value);

LIST_TYPE pop_stack(LinkedList *stack);

LIST_TYPE peek_stack(const LinkedList *stack);

// ОЧЕРЕДЬ

void enqueue(LinkedList *queue, LIST_TYPE value);

LIST_TYPE dequeue(LinkedList *queue);

LIST_TYPE peek_queue(const LinkedList *queue);

#endif