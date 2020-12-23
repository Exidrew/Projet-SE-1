#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <pthread.h>

#include "headers/server.h"
#include "headers/error.h"
#include "headers/message.h"
#include "headers/authentification.h"
#include "headers/tinyShell.h"
#include "headers/gestionChaine.h"

#define MAXIMUM 100

int sockets[SOMAXCONN];

static void* ping(void* p_data) {
    printf("Entre fonction\n");
    int index = (*(int*)p_data)-1;
    int sock = sockets[index];
    AuthMessage authRequest;
    AuthMessage authResult;
    Message receive;
    char** commandes = allouerMemoireCommandes();
    int status = 0;
    int nbCommandes = 0;
    
    authResult.header = SSH_MSG_USERAUTH_FAILURE;
    while(authResult.header != SSH_MSG_USERAUTH_SUCCESS) {
        receiveTCP(sock, &authRequest, sizeof(authRequest));
        printf("Le header recu : %d\n", authRequest.header);
        printf("Password recu: %s\n", authRequest.methodFields);

        switch (authRequest.header) {
            case SSH_MSG_USERAUTH_REQUEST:
                if (!strncmp(authRequest.methodFields, "mdp", strlen("mdp"))) authResult.header = SSH_MSG_USERAUTH_SUCCESS;
                sprintf(authResult.methodFields, "%s", "Mot de passe incorrect");
                break;
            default:
                sprintf(authResult.methodFields, "%s", "Authentification requise");
                break;
        }
        sendTCP(sock, &authResult, sizeof(authResult));
    }

    printf("Test avant boucle\n");

    for (;;) {
        printf("Attend un TCP\n");
        receiveTCP(sock, &receive, sizeof(receive));
        printf("Test avant 0\n");

        printf("Client %d :\n", index);
        printf("Le header recu : %d\n", receive.header);
        printf("Le type recu : %s\n", receive.type);
        printf("Recu : %s\n", receive.command);
        if (!strncmp(receive.command, "exit", strlen("exit"))) {
            printf("Client disconnected\n");
            break;
        }
        
        switch(receive.header) {
            case SSH_MSG_CHANNEL_REQUEST:
                printf("Gestion du channel request\n");
                commandes = gererChaine("ls -l", commandes, &nbCommandes);
                printf("Passe !\n");
                executerCommande(commandes, 1, &status);
                wait(NULL);
                printf("Termine le executerCommande");
                break;
            default:
                printf("Gestion de l'erreur ici\n");
                break;
        }
        
        sendTCP(sock, &receive, sizeof(receive));
    }
    pthread_exit(0);
}

int main(void) {
    int i = 0, t;

    Server server1 = createServerTCP();
    server1->bind(server1, 1344);

    // SOMMAXCONN est le nombre maximum de client accepté en C
    if ((listen(server1->socket, SOMAXCONN)) == ERR) syserror(LISTEN_ERR);
    printf("Listen OK\n");
    
    // Multiclient
    for(;;) {
        t=i;
        sockets[i] = accept(server1->socket, (struct sockaddr*)&server1->clientAddr, &server1->len);
        printf("Accepted client %d\n", t);

        pthread_t thread1;
        printf("Test : %d\n", t);
        pthread_detach(thread1);
        pthread_create(&thread1, NULL, ping, &t);
        i++;
    }
    printf("Test\n");
    destroyServer(server1);
    return 0;
}
