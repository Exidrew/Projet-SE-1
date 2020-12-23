#ifndef MESSAGE_H
#define MESSAGE_H

#define SSH_MSG_CHANNEL_REQUEST 98
#define SSH_MSG_CHANNEL_SUCCESS 99
#define SSH_MSG_CHANNEL_FAILURE 100

#define TYPE_EXEC "exec"
#define TYPE_SIGNAL "signal"

#ifndef BYTE_DEF
#define BYTE_DER
typedef unsigned char byte;
#endif

#define SIZE_CMD 2048

struct message {
    byte header;
    char type[8];
    char command[SIZE_CMD];
} __attribute__((packed));

typedef struct message Message;

#endif