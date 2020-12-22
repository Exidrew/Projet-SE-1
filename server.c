#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "headers/error.h"
#include "headers/server.h"
#include "headers/message.h"

/* ============= Fonctions privées ============= */

static ssize_t serverReceiveTCP(struct server* this, void* message, size_t size) {
    // Pas obligé de préciser le client avec qui on discute
    // Car la connexion est établie physiquement
    return recv(this->acceptedSocket, message, size, 0);
}

static void serverSendTCP(struct server* this, void* message, size_t size){
    // Pas obligé de préciser le client avec qui on discute
    // Car connexion établie physiquement
    if (send(this->acceptedSocket, message, size, 0) == ERR) {
        neterr_server(this, SEND_ERR);
    }
}

static void serverBind(struct server* this,int port){
    this->servAddr.sin_family = AF_INET; // On est dans un protocole internet
    this->servAddr.sin_addr.s_addr = INADDR_ANY; // Accepte connexion de localhost et de l'exterieur
    this->servAddr.sin_port = htons((uint16_t) port); // Affecte le port via htons
    // Bind le socket
    if (bind(this->socket, (struct sockaddr *) &this->servAddr, sizeof(this->servAddr)) < 0) {
        neterr_server(this, BINDING_ERR);
    }
}

/* ============= Fonctions publiques ============= */

Server createServerTCP(){
    Server server = calloc(1, sizeof(struct server));

    int sfd;
    if((sfd = socket(AF_INET,SOCK_STREAM,0)) == ERR){
        free(server);
        syserror(SOCKET_ERR);
    }

    server->socket = sfd;
    server->bind = &serverBind;
    server->receive = &serverReceiveTCP;
    server->send = &serverSendTCP;
    memset(&server->servAddr, 0, sizeof(struct sockaddr_in));
    memset(&server->clientAddr, 0, sizeof(struct sockaddr_in));
    server->len=sizeof(struct sockaddr_in);
    return server;
}

ssize_t receiveTCP(int socket, void *message, size_t size) {
    return recv(socket, message, size, 0);
}

void sendTCP(int socket, void* message, size_t size) {
    if (send(socket, message, size, 0) == ERR) {
        syserror(SEND_ERR);
    }
}

void destroyServer(Server this) {
    close(this->socket);
    free(this);
}