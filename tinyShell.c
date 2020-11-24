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
    //afficherLesCommandesEntrees(tabcmd, nbCommandes);
    for (int i = 0; i < nbCommandes; i++) {
        if (tabcmd == NULL || tabcmd[i] == NULL) {
            *status = -1;
            return;
        }

        if (estCommande(tabcmd[i], CMD_SETVARIABLE)) *status = setVariableLocale(tabcmd[i]);
        else if (estCommande(tabcmd[i], CMD_DELVARIABLE)) *status = delVariableLocale(tabcmd[i]);
        else if (estCommande(tabcmd[i], CMD_PRINTVARIABLE)) *status = afficherVariablesLocales();
        else if (estCommande(tabcmd[i], CMD_CD)) executerCd(tabcmd[i], nbCommandes);
        else executerProgrammeExterne(tabcmd[i]);
    }
}

void recupererNomProgramme(char nomProgramme[100], char* commande) {
    int i;
    for (i = 0; commande[i] != ' ' && commande[i] != '\n' && commande[i] != '\0'; i++) {
        nomProgramme[i] = commande[i];
    } 
}

void executerProgrammeExterne(char* commande) {
    char repertory[100], nomProgramme[100];
    getPwd(repertory);
    strcat(repertory, "/");
    recupererNomProgramme(nomProgramme, commande);
    strcat(repertory, nomProgramme);

    if(fork()==0) {
        char* arg[2] = {commande, NULL};
        execvp(repertory, arg);
        syserror(EXEC_FAIL);
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
        if (commandes != NULL) viderCommande(commandes);
    }
    freeTout(commandes);
    exit(0);
}