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
#include "headers/tubeCommunication.h"

extern char** environ;
int tube[2];

void afficherRetour(char** tabcmd, int nbCommandes, int nbRedirection, int status) {
    int i, retour = 0;
    for (i=0; i < nbCommandes - nbRedirection; i++) {
        wait(&status);
        retour = WEXITSTATUS(status);
        if (WIFEXITED(status) && status == FAIL_EXEC) {
            printf(ROUGE("Abnormal exit of ["));
            afficherEnBrutLesCommandesEntrees(tabcmd, nbCommandes);
            printf(ROUGE("\b]=%d\n"), status);
            return;
        }
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

int executerCommande(char** tabcmd, int nbCommandes, int* status) {
    printf("Entre dans l'execution avec les commandes :\n");
    pid_t pid = getpid();
    int redirection = 0, finDeRedirection = 0, nbRedirection = 0, i;
    tabcmd = remplacerLesVariablesDansLesCommandes(tabcmd, nbCommandes, status);
    //afficherLesCommandesEntrees(tabcmd, nbCommandes);
    for (i = 0; i < nbCommandes; i++) {
        printf("La commande : %s\n", tabcmd[i]);
        if (getpid() == pid) {
            if (tabcmd == NULL || tabcmd[i] == NULL) {
                *status = -1;
                return nbRedirection;
            }

            if (i+1 < nbCommandes && !strcmp(tabcmd[i+1], "|")) {
                redirection += 1;
                nbRedirection += 1;
            } else if (redirection && strcmp(tabcmd[i], "|")) {
                finDeRedirection = 1;
                redirection = 0;
            } else if (strcmp(tabcmd[i], "|")) redirection = 0;

            if (nbRedirection) wait(status);

            if (estSeparateur(tabcmd[i])) continue;
            else if (estCommande(tabcmd[i], CMD_SETVARIABLE)) *status = setVariableLocale(tabcmd[i]);
            else if (estCommande(tabcmd[i], CMD_DELVARIABLE)) *status = delVariableLocale(tabcmd[i]);
            else if (estCommande(tabcmd[i], CMD_PRINTVARIABLE)) *status = afficherVariablesLocales();
            else if (estCommande(tabcmd[i], CMD_CD)) executerCd(tabcmd[i], nbCommandes);
            else executerProgrammeExterne(tabcmd[i], redirection, finDeRedirection, status);
        } else {
            exit(0);
        }
    }
    return nbRedirection;
}

void executerProgrammeExterne(char* commande, int redirection, int finDeRedirection, int* status) {
    char repertory[100], nomProgramme[100];
    char* args[64];
    int i, nbArgs;

    if (redirection == 1) if (pipe(tube) == ERR) fatalsyserror(CREATION_PIPE_FAILED);

    // Ces memset evitent une fuite de memoire valgrind
    memset(repertory, '\0', 100);
    memset(nomProgramme, '\0', 100);
    
    if (estCommandeMy(commande)) {
        // Ici nos commandes qu'on a implémenté
        getPwd(repertory);
        strcat(repertory, "/");
        recupererNomProgramme(nomProgramme, commande);
        strcat(repertory, nomProgramme);

        if(fork()==0) {
            if (redirection) {
                if (redirection >= 2) {
                    lireEcrireVersStandard(tube);
                    *status = execlp(repertory, commande, "l", NULL);
                } else {
                    ecrireVersSortieStandard(tube);
                    *status = execlp(repertory, commande, NULL);
                }
            } else if (finDeRedirection) {
                lireDepuisEntreeStandard(tube);
                *status = execlp(repertory, commande, "l", NULL);
            } else *status = execlp(repertory, commande, NULL);
            fatalsyserror(EXEC_FAIL);
        }
    } else {
        if(fork()==0) {
            // Ici on peut executer des commandes du shell de base
            // On doit quand même gérer les redirections ici
            recupererNomProgramme(nomProgramme, commande);
            for (i=0; i < 64; i++) args[i] = (char*) calloc(100, sizeof(char));
            nbArgs = recupererArguments(args, commande);
            free(args[nbArgs+1]);
            args[nbArgs+1] = NULL;
            if (redirection) {
                if (redirection >= 2) {
                    lireEcrireVersStandard(tube);
                    *status = execvp(nomProgramme, args);
                } else {
                    ecrireVersSortieStandard(tube);
                    *status = execvp(nomProgramme, args);
                }
            } else if (finDeRedirection) {
                lireDepuisEntreeStandard(tube);
                *status = execvp(nomProgramme, args);
            } else *status = execvp(nomProgramme, args);
            for (i=0; i < 64; i++) free(args[i]);
            fatalsyserror(EXEC_FAIL);
        }
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

#ifndef SERVER
int main(void) {
    char** commandes;
    int nbCommandes;
    int status = 0;
    int nbRedirection;

    commandes = allouerMemoireCommandes();

    for(;;) {
        status = 0;
        commandes = demanderCommande(commandes, &nbCommandes);
        if (!strcmp(*commandes, CMD_EXIT)) {
            freeCommandes(commandes);
            exit(0);
        }
        nbRedirection = executerCommande(commandes, nbCommandes, &status);
        afficherRetour(commandes, nbCommandes, nbRedirection, status);
        commandes = viderCommande(commandes);
    }
    freeTout(commandes);
    exit(0);
}
#endif