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

void afficherRetour(char** tabcmd, int nbCommandes, int status) {
    int i, retour = 0;
    for (i=0; i < nbCommandes; i++) {
        wait(&status);
        if (retour != FAIL_EXEC) retour = WEXITSTATUS(status);
    }
    if (WIFEXITED(status)) {
        if (retour != FAIL_EXEC) {
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
    pid_t pid = getpid();
    int redirection = 0;
    tabcmd = remplacerLesVariablesDansLesCommandes(tabcmd, nbCommandes, status);
    //afficherLesCommandesEntrees(tabcmd, nbCommandes);
    for (int i = 0; i < nbCommandes; i++) {
        if (getpid() == pid) {
            if (tabcmd == NULL || tabcmd[i] == NULL) {
                *status = -1;
                return;
            }

            if (i+1 < nbCommandes && !strcmp(tabcmd[i+1], "|")) redirection = 1;
            else redirection = 0;

            if (redirection) printf("Redirection !\n");

            if (estSeparateur(tabcmd[i])) continue;
            else if (estCommande(tabcmd[i], CMD_SETVARIABLE)) *status = setVariableLocale(tabcmd[i]);
            else if (estCommande(tabcmd[i], CMD_DELVARIABLE)) *status = delVariableLocale(tabcmd[i]);
            else if (estCommande(tabcmd[i], CMD_PRINTVARIABLE)) *status = afficherVariablesLocales();
            else if (estCommande(tabcmd[i], CMD_CD)) executerCd(tabcmd[i], nbCommandes);
            else executerProgrammeExterne(tabcmd[i]);
        } else {
            exit(0);
        }
    }
}

void recupererNomProgramme(char nomProgramme[100], char* commande) {
    int i;
    for (i = 0; commande[i] != ' ' && commande[i] != '\n' && commande[i] != '\0'; i++) {
        nomProgramme[i] = commande[i];
    }
}

int recupererArguments(char* args[], char* commande) {
    int i, indice = 0, ind = 0;
    for (i=0; i < strlen(commande); i++) {
        if (isspace(commande[i])) {
                indice++;
                ind = 0;
            continue;
        }
        args[indice][ind++] = commande[i];
    }
    return indice;
}

void executerProgrammeExterne(char* commande) {
    char repertory[100], nomProgramme[100];
    char* args[64];
    int i, nbArgs;
    // Ces memset evites une fuite de memoire valgrind
    memset(repertory, '\0', 100);
    memset(nomProgramme, '\0', 100);
    for (i=0; i < 64; i++) {
        args[i] = (char*) calloc(100, sizeof(char));
        memset(args[i], '\0', 100);
    }
    
    if (!strncmp(commande, "my", 2)) {
        getPwd(repertory);
        strcat(repertory, "/");
        recupererNomProgramme(nomProgramme, commande);
        strcat(repertory, nomProgramme);

        if(fork()==0) {
            execlp(repertory, commande, NULL);
            syserror(EXEC_FAIL);
        }
    } else {
        recupererNomProgramme(nomProgramme, commande);
        nbArgs = recupererArguments(args, commande);
        args[nbArgs+1] = NULL;
        if(fork()==0) {
            execvp(nomProgramme, args);
            syserror(EXEC_FAIL);
        }
    }
    for (i=0; i < 64; i++) {
        free(args[i]);
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
        afficherLesCommandesEntrees(commandes, nbCommandes);
        executerCommande(commandes, nbCommandes, &status);
        afficherRetour(commandes, nbCommandes, status);
        if (commandes != NULL) viderCommande(commandes);
    }
    freeTout(commandes);
    exit(0);
}