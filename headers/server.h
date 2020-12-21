#ifndef SERVER_H
#define SERVER_H

#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define neterr_server(srv, n) destroyServer(srv), syserror(n);

struct server {
    int socket;
    int acceptedSocket;
    struct sockaddr_in servAddr;
    struct sockaddr_in clientAddr;
    socklen_t len;

    ssize_t (*receive)(struct server* this, char* buff, size_t size);
    void (*send)(struct server* this, char* message);
    void (*bind)(struct server* this, int port);
};

typedef struct server* Server;

Server createServerTCP();
void destroyServer(Server this);
void sendTCP(int socket, char* msg);
ssize_t receiveTCP(int socket, char* buff, size_t size);

#endif