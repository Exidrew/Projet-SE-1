#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>

#include "headers/tinyShell.h"
#include "headers/error.h"
#include "headers/memoirePartagee.h"
#include "headers/gestionChaine.h"
#include "headers/variables.h"
#include "headers/variablesLocales.h"

int idTab, idVar;
key_t clefTab, clefVar;
TableauVariables* tab;

extern char** environ;

void afficherRetour(char** tabcmd, int nbCommandes) {
    int status;
    wait(&status);
    if (WIFEXITED(status)) {
        if ((status = WEXITSTATUS(status)) != FAIL_EXEC) {
            printf(VERT("exit status of ["));
            afficherEnBrutLesCommandesEntrees(tabcmd, nbCommandes);
            printf(VERT("\b]=%d\n"), status);
        }
    }
    else puts(ROUGE("Abnormal exit"));
}

void executerCommande(char** tabcmd, int nbCommandes) {
    afficherLesCommandesEntrees(tabcmd, nbCommandes);
    for (int i = 0; i < nbCommandes; i++) {
        if (estCommande(tabcmd[i], CMD_SETVARIABLE)) setVariableLocale(tabcmd[i], tab);
        else if (estCommande(tabcmd[i], CMD_DELVARIABLE)) delVariableLocale(tabcmd[i], tab);
        else {
            execlp(*tabcmd, *tabcmd, NULL);
            freeCommandes(tabcmd);
            freeVariables(tab);
            syserror(2);
            exit(FAIL_EXEC);
        }
    }
    shmdt(tab->variables);
    shmdt(tab);
    exit(0);
}

void freeCommandes(char** commandes) {
    int i;
    for (i = 0; i < sizelgcmd; i++) {
        free(commandes[i]);
    }
    free(commandes);
}

int main(void) {
    char** commandes;
    pid_t pid;
    int nbCommandes;

    allouerMemoirePartagee(clefTab, idTab, 0, TableauVariables, tab, 1);
    allouerMemoirePartagee(clefVar, idVar, 1, Variables, tab->variables, 5);
    commandes = allouerMemoireCommandes();

    for(;;) {
        commandes = demanderCommande(commandes, &nbCommandes);
        if (!strcmp(*commandes, CMD_EXIT)) {
            detruireMemoirePartagee(idVar, tab->variables);
            detruireMemoirePartagee(idTab, tab);
            freeCommandes(commandes);
            exit(0);
        }
        if ((pid = fork()) == ERR) {
            detruireMemoirePartagee(idVar, tab->variables);
            detruireMemoirePartagee(idTab, tab);
            freeCommandes(commandes);
            fatalsyserror(1);
        }
        if(!pid) executerCommande(commandes, nbCommandes);
        else {
            afficherRetour(commandes, nbCommandes);
            afficherVariables(tab);
            viderCommande(commandes);
        }
    }
    detruireMemoirePartagee(idVar, tab->variables);
    detruireMemoirePartagee(idTab, tab);
    freeCommandes(commandes);
    exit(0);
}