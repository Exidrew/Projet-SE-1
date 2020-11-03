#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>

#include "headers/tinyShell.h"
#include "headers/error.h"
#include "headers/variables.h"
#include "headers/tubeCommunication.h"

void afficherRetour(char** tabcmd) {
    char** ps;
    int status;
    wait(&status);
    if (WIFEXITED(status)) {
        if ((status = WEXITSTATUS(status)) != FAIL_EXEC) {
            printf(VERT("exit status of ["));
            for (ps = tabcmd; *ps; ps++) printf("%s ", *ps);
            printf(VERT("\b]=%d\n"), status);
        }
    }
    else puts(ROUGE("Abnormal exit"));
}

void demanderCommande(char** tabcmd) {
    char lgcmd[sizelgcmd], *s;
    int i;
    putchar('>');
    fgets(lgcmd, sizelgcmd-1, stdin);
    for(s=lgcmd; isspace(*s); s++);
    for(i=0; *s;i++) {
        tabcmd[i] = s;
        while(!isspace(*s)) s++;
        *s++='\0';
        while (isspace(*s)) s++;
    }
    if (i) tabcmd[i] = NULL;
}

void executerSetVariable(char** tabcmd) {
        ++tabcmd; // Vire le "set"
        listeVariables = setVariable(tabcmd, listeVariables);
        ecrireVariableVersTube(tubeSetVariable, listeVariables);
        exit(0);
}

void executerCommande(char** tabcmd) {
    if (!strcmp(*tabcmd, CMD_SETVARIABLE)) executerSetVariable(tabcmd);
    execvp(*tabcmd, tabcmd);
    syserror(2);
    exit(FAIL_EXEC);
}

int main(void) {
    char* tabcmd[size];
    pid_t pid;

    for(;;) {
        creerTubeDeCommunication(tubeSetVariable);
        demanderCommande(tabcmd);
        if (!strcmp(*tabcmd, "exit")) exit(0);
        if ((pid = fork()) == ERR) {
            fatalsyserror(1);
        }
        if(!pid) executerCommande(tabcmd);
        else {
            afficherRetour(tabcmd);
            lireVariableDepuisTube(tubeSetVariable);
            fermerTube(tubeSetVariable);
            afficher_variables(listeVariables);
        }
    }
    exit(0);
}