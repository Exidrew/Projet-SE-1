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
#include "headers/cd.h"

extern char** environ;

void afficherRetour(char** tabcmd, int nbCommandes,int status) {
    wait(&status);
    if (WIFEXITED(status)) {
        if ((status = WEXITSTATUS(status)) != FAIL_EXEC) {
            printf(VERT("exit status of ["));
            afficherEnBrutLesCommandesEntrees(tabcmd, nbCommandes);
            printf(VERT("\b]=%d\n"), status);
        }
    }
    else {
        printf(ROUGE("Abnormal exit of ["));
        afficherEnBrutLesCommandesEntrees(tabcmd, nbCommandes);
        printf(ROUGE("\b]=%d\n"), status);
    }
}

void executerCommande(char** tabcmd, int nbCommandes, int* status) {
    tabcmd = remplacerLesVariablesDansLesCommandes(tabcmd, nbCommandes, status);
    printf("Continue\n");
    //afficherLesCommandesEntrees(tabcmd, nbCommandes);
    for (int i = 0; i < nbCommandes; i++) {
        if (tabcmd == NULL || tabcmd[i] == NULL) {
            printf("Entre ici\n");
            *status = -1;
            return;
        }

        if (estCommande(tabcmd[i], CMD_SETVARIABLE)) *status = setVariableLocale(tabcmd[i]);
        else if (estCommande(tabcmd[i], CMD_DELVARIABLE)) *status = delVariableLocale(tabcmd[i]);
        else if (estCommande(tabcmd[i], CMD_CD)) executerCd(tabcmd[i], nbCommandes);
        // else {
        //     printf("ENTRE DANS LE ELSE");
        //     execlp(*tabcmd, *tabcmd, NULL);
        //     printf("ENTRE DANS LE EXECLP");
        //     freeCommandes(tabcmd);
        //     freeVariables(tab);
        //     syserror(2);
        //     exit(FAIL_EXEC);
        // }
    }
}

void freeCommandes(char** commandes) {
    int i;
    for (i = 0; i < sizelgcmd; i++) {
        free(commandes[i]);
    }
    free(commandes);
}

void freeTout(char** commandes) {
    freeCommandes(commandes);
}

int main(void) {
    char** commandes;
    int nbCommandes;
    int status;

    commandes = allouerMemoireCommandes();

    for(;;) {
        commandes = demanderCommande(commandes, &nbCommandes);
        if (!strcmp(*commandes, CMD_EXIT)) {
            freeCommandes(commandes);
            exit(0);
        }
        executerCommande(commandes, nbCommandes, &status);
        afficherRetour(commandes, nbCommandes, status);
        printf("Test\n");
        if (commandes != NULL) viderCommande(commandes);
    }
    freeTout(commandes);
    exit(0);
}