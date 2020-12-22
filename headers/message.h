#ifndef MESSAGE_H
#define MESSAGE_H

struct message {
    int taille;
    char msg[4096];
} __attribute__((packed));

typedef struct message Message;

#endif