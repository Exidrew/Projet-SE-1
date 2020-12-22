#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <pthread.h>

#include "headers/server.h"
#include "headers/error.h"
#include "headers/message.h"
#include "headers/authentification.h"

#define MAXIMUM 100

int sockets[SOMAXCONN];

static void* ping(void* p_data) {
    printf("Entre fonction\n");
    int index = (*(int*)p_data)-1;
    int sock = sockets[index];
    AuthMessage authRequest;
    AuthMessage authResult;
    Message *receive;
    printf("Avant\n");
    receiveTCP(sock, &authRequest, sizeof(authRequest));
    printf("lol\n");
    printf("Type recu : %d\n", authRequest.type);
    printf("Password recu: %s\n", authRequest.methodFields);

    authResult.type = SSH_MSG_USERAUTH_SUCCESS;
    sendTCP(sock, &authResult, sizeof(authResult));

    printf("Test avant boucle\n");

    for (;;) {
        receive = calloc(1, sizeof(Message));

        receiveTCP(sock, receive, sizeof(Message));
        printf("Test avant 0\n");

        printf("Client %d :\n", index);
        printf("Recu : %s\n", receive->msg);
        printf("Resultat exit : %d\n", !strncmp(receive->msg, "exit\n", strlen("exit\n")));
        if (!strncmp(receive->msg, "exit", strlen("exit"))) {
            printf("Client disconnected\n");
            break;
        }
        
        sendTCP(sock, receive, sizeof(receive));
        free(receive);
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
