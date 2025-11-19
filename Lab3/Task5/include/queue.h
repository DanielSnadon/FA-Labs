#ifndef QUEUE_H
#define QUEUE_H

#include "structures.h"

Node* mergeQueue(Node *q1, Node *q2);

Node* addQueue(Node *q, Mail m);

Node* popQueue(Node *q, Mail *m);

int isQueueEmpty(Node *q);

void freeQueue(Node *q);

#endif