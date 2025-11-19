#include "queue.h"
#include <stdio.h>

// Вспомогательные функции

void swap(Node *n)
{
    if (n != NULL) {
        Node *temp = n->left;
        n->left = n->right;
        n->right = temp;
    }
}

// Основные функции

Node* mergeQueue(Node *q1, Node *q2)
{
    if (q1 == NULL) {
        return q2;
    }
    if (q2 == NULL) {
        return q1;
    }

    if (q1->data.priority < q2->data.priority) {
        Node *temp = q1;
        q1 = q2;
        q2 = temp;
    }

    swap(q1);

    q1->right = mergeQueue(q1->right, q2);

    return q1;
}

Node* addQueue(Node *q, Mail m)
{
    Node *newNode = (Node*)malloc(sizeof(Node));

    if (newNode == NULL) {
        printf("Ошибка: не удалось создать очередь необходимого размера. \n");
        return q;
    }

    newNode->data = m;
    newNode->left = NULL;
    newNode->right = NULL;

    return mergeQueue(q, newNode);
}

Node* popQueue(Node *q, Mail *m)
{
    if (q == NULL) {
        return NULL;
    }

    if (q != NULL) {
        *m = q->data;
    }

    Node *left = q->left;
    Node *right = q->right;

    free(q);

    return mergeQueue(left, right);
}

int isQueueEmpty(Node *q)
{
    return (q == NULL);
}

void freeQueue(Node *q)
{
    if (q == NULL) {
        return;
    }

    freeQueue(q->left);
    freeQueue(q->right);
    free(q);
}