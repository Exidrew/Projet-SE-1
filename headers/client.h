#ifndef CLIENT_H
#define CLIENT_H

#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define LOCAL_IP "127.0.0.1"
#define PORT 1344

#ifndef MAXIMUM
#define MAXIMUM 100
#endif

#define neterr_client(clt, n) destroyClient(clt),syserror(n);

struct client {
    int socket;
    struct sockaddr_in clientAddr;
    socklen_t taille;

    // Fonctions
    ssize_t (*receive)(struct client* this, char* buff, size_t size);
    void (*send)(struct client* this, char* message);
};

typedef struct client* Client;

Client createClientTcp(char* addr, int port);
void destroyClient(Client this);
void lancerClient();

#endif