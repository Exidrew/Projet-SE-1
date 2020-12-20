#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#define MAXIMUM 1024
#define LOCAL_IP "127.0.0.1"
#define PORT 1344

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

#endif