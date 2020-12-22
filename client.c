#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "headers/error.h"
#include "headers/client.h"
#include "headers/gestionChaine.h"

/* ============= Fonctions privées ============= */

static ssize_t clientReceiveTCP(Client this, char* buff, size_t size) {
    if (!buff) return 0;
    return recv(this->socket, buff, size, 0);
}

static void clientSendTCP(Client this, char* message) {
    if (send(this->socket, message, strlen(message), 0) == ERR) neterr_client(this, SEND_ERR);
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

void lancerClient() {
    char* buffer_send = calloc(MAXIMUM, sizeof(char));
    char buffer_recv[MAXIMUM];
    ssize_t tailleMessageRecu;

    memset(buffer_recv, 0, sizeof(char) * MAXIMUM);

    Client client = createClientTcp(LOCAL_IP, PORT);

    if (connect(client->socket, (const struct sockaddr*) &(client->clientAddr), sizeof(struct sockaddr_in)) == ERR) {
        destroyClient(client);
        fatalsyserror(25);
    }

    for (;;) {
        prompt(buffer_send, "Entrez votre message : ");
        if (!strncmp(buffer_send, "exit", strlen("exit"))) {
            client->send(client, buffer_send);
            break;
        }
        printf("Le message : %s\n", buffer_send);

        client->send(client, buffer_send);

        tailleMessageRecu = client->receive(client, buffer_recv, MAXIMUM);
        buffer_recv[tailleMessageRecu] = '\0';
        printf("Recu : %s", buffer_recv);
    }
    free(buffer_send);
}