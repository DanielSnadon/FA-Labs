#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef enum {
    STATUS_IN_WAY,
    STATUS_DELIVERED,
    STATUS_UNDELIVERED
} MailStatus;

typedef struct {
    unsigned int id;
    char type[64];
    MailStatus status;
    int priority;
    unsigned int fromId;
    unsigned int toId;
    unsigned int currentId;
    char data[128];
} Mail;

typedef struct Node {
    Mail data;
    struct Node *left;
    struct Node *right;
} Node;

typedef struct {
    unsigned int id;
    unsigned int maxMails;
    unsigned int currentMails;
    Node *mails;
    unsigned int *connections;
    unsigned int connectionCount;
} Station;

#endif