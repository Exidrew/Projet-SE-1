#ifndef AUTHENTIFICATION_H
#define AUTHENTIFICATION_H

#define SSH_MSG_USERAUTH_REQUEST 50
#define SSH_MSG_USERAUTH_FAILURE 51
#define SSH_MSG_USERAUTH_SUCCESS 52

#define USERNAMELENGTH 50
#define SERVICENAMELENGTH 50
#define METHODNAMELENTGTH 50
#define METHODFIELDSLENGTH 1024

#define PUBLIC_KEY "publickey"
#define PASSWORD "password"
#define HOSTBASED "hostbased"
#define NONE "none"

typedef unsigned char byte;

struct auth {
    byte type;
    char userName[USERNAMELENGTH];
    char serviceName[SERVICENAMELENGTH];
    char methodName[METHODNAMELENTGTH];
    char methodFields[METHODFIELDSLENGTH];
} __attribute__((packed));

typedef struct auth AuthMessage;

#endif