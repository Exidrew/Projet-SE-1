#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "headers/error.h"
#include "headers/client.h"
#include "headers/gestionChaine.h"
#include "headers/message.h"
#include "headers/authentification.h"

/* ============= Fonctions privées ============= */

static ssize_t clientReceiveTCP(Client this, void* message, size_t size) {
    if (!message) return 0;
    return recv(this->socket, message, size, 0);
}

static void clientSendTCP(Client this, void* message, size_t size) {
    if (send(this->socket, message, size, 0) == ERR) neterr_client(this, SEND_ERR);
}

/* ============= Fonctions publiques ============= */

Client createClientTcp(char* addr, int port) {
    Client client;
    int sock;
    if ((client = (Client) calloc(1, sizeof(struct client))) == NULL) fatalsyserror(MEM_FAILED_ALLOCATION);
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == ERR) neterr_client(client, SOCKET_ERR);

    memset(&client->clientAddr, 0, sizeof(struct sockaddr_in));

    client->socket = sock;
    client->clientAddr.sin_family = AF_INET;
    client->clientAddr.sin_port = htons((uint16_t) port);
    client->receive = &clientReceiveTCP;
    client->send = &clientSendTCP;
    client->taille = sizeof(struct sockaddr_in);

    if (!inet_aton(addr, &client->clientAddr.sin_addr)) neterr_client(client, SOCKET_ERR);

    return client;
}

void destroyClient(Client this) {
    close(this->socket);
    free(this);
}

void lancerClient(char* clientInfo) {
    Message* send, *receive;
    ssize_t tailleMessageRecu;
    AuthMessage authMessage;
    AuthMessage authResult;
    Client client = createClientTcp(LOCAL_IP, PORT);

    if (connect(client->socket, (const struct sockaddr*) &(client->clientAddr), sizeof(struct sockaddr_in)) == ERR) {
        destroyClient(client);
        fatalsyserror(25);
    }

    authMessage.type = SSH_MSG_USERAUTH_REQUEST;
    prompt(authMessage.methodFields, "Password: ", METHODFIELDSLENGTH);
    sprintf(authMessage.userName, "%s", clientInfo);
    sprintf(authMessage.methodName, "%s", PASSWORD);
    sprintf(authMessage.serviceName, "%s", clientInfo);

    printf("Le mot de passe : %s\n", authMessage.methodFields);

    client->send(client, &authMessage, sizeof(authMessage));
    client->receive(client, &authResult, sizeof(authResult));

    printf("Resultat authentification: %d\n", authResult.type);

    for (;;) {
        send = calloc(1, sizeof(Message));
        receive = calloc(1, sizeof(Message));

        prompt(send->msg, "Entrez votre message : ", MAXIMUM);
        if (!strncmp(send->msg, "exit", strlen("exit"))) {
            client->send(client, send, sizeof(send));
            break;
        }
        printf("Le message : %s\n", send->msg);

        client->send(client, send, sizeof(send));

        tailleMessageRecu = client->receive(client, receive, sizeof(receive));
        receive->msg[tailleMessageRecu] = '\0';
        printf("Recu : %s\n", receive->msg);
        free(send);
        free(receive);
    }
}