#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <pthread.h>

#include "headers/server.h"
#include "headers/error.h"

#define MAX 500

int sockets[SOMAXCONN];

static void* ping(void* p_data) {
    int index = (*(int*)p_data)-1;
    printf("Client %d\n", index);
    int s = sockets[index];
    char* msg;
    char buf[MAX];

    for (;;) {
        memset(buf, 0, sizeof(char)*MAX);
        ssize_t n = receiveTCP(s, buf, MAX);
        buf[n] ='\0';
        char* str = "PING";

        if(strncmp(buf, str, strlen(str))==0){
            msg = "PONG";
        }else {
            msg = "PAS PONG";
        }
        printf("%s\n", buf);
        sendTCP(s, msg);
    }
}

int main(void) {
    int i = 0, t;
    char buf[MAX];
    char *msg=NULL;
    memset(buf, 0, sizeof(char)*MAX);

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
        pthread_create(&thread1, NULL, ping, &t);
        pthread_detach(thread1);
        i++;
    }
    destroyServer(server1);
    return 0;
}
