#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <regex.h>
#include <string.h>

#include "headers/error.h"
#include "headers/myssh.h"

#define neterr_client(clt, n) destroyClient(clt),syserror(n);

/* ============= Fonctions privées ============= */
int checkIdentification(char* id) {
    regex_t regex;
    const char schema[43] = "^[a-zA-Z0-9_]+@([0-9]{1,3}.){3}[0-9]{1,3}$";

    if (regcomp(&regex, schema, REG_EXTENDED)) {
        regfree(&regex);
        fatalsyserror(8);
    }
    
    if (regexec(&regex, id, 0, NULL, 0)) {
        printf("Invalid User/password for host1\n");
        regfree(&regex);
        return 0;
    }
    regfree(&regex);
    return 1;
}

int checkArguments(int nbArgs) {
    return nbArgs == 2;
}

void prompt(char* chaine, char* message) {
    size_t max = MAXIMUM;
    size_t size = 0;
    char car;

    printf("%s", message);

    while(1) {
        car = getchar();
        size++;

        if (size >= max) {
            chaine = (char*) realloc(chaine, (size * 2) * sizeof(char));
            if (chaine == NULL) fatalsyserror(MEM_FAILED_ALLOCATION);
            max = size * 2;
        }
        chaine[size-1] = (char) car;
        
        if ((char) car == '\n') {
            chaine[size] = '\0';
            break;
        }
    }
}

void printUsage() {
    printf("Usage : myssh username@host\n");
    exit(0);
}

static ssize_t clientReceiveTCP(Client this, char* buff, size_t size) {
    if (!buff) return 0;
    return recv(this->socket, buff, size, 0);
}

static void clientSendTCP(Client this, char* message) {
    if (send(this->socket, message, strlen(message), 0) == ERR) neterr_client(this, SEND_ERR);
}

void lancerClient() {
    char* buffer_send = calloc(MAXIMUM, sizeof(char));
    char buffer_recv[MAXIMUM];

    memset(buffer_recv, 0, sizeof(char) * MAXIMUM);

    Client client = createClientTcp(LOCAL_IP, PORT);

    // if (connect(client->socket, (const struct sockaddr*) &(client->clientAddr), sizeof(struct sockaddr_in)) == ERR) {
    //     destroyClient(client);
    //     fatalsyserror(25);
    // }

    for (;;) {
        prompt(buffer_send, "Entrez votre message : ");
        if (!strncmp(buffer_send, "exit", strlen("exit"))) break;

        printf("Le message : %s\n", buffer_send);
    }
    free(buffer_send);
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

int main(int argc, char* argv[]) {
    char* password = calloc(MAXIMUM, sizeof(char));
    if (!checkArguments(argc)) printUsage();
    if (!checkIdentification(argv[1])) printUsage();

    prompt(password, "Password: ");

    printf("Le password : %s\n", password);
    printf("Test\n");
    lancerClient();

    free(password);
    return 0;
}