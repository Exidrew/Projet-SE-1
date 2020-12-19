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
    printf("Termine retour\n");
}

int executerCommande(char** tabcmd, int nbCommandes, int* status) {
    pid_t pid = getpid();
    int redirection = 0, finDeRedirection = 0, nbRedirection = 0;
    tabcmd = remplacerLesVariablesDansLesCommandes(tabcmd, nbCommandes, status);
    //afficherLesCommandesEntrees(tabcmd, nbCommandes);
    for (int i = 0; i < nbCommandes; i++) {
        printf("nb : %d\n", nbCommandes);
        if (getpid() == pid) {
            if (tabcmd == NULL || tabcmd[i] == NULL) {
                *status = -1;
                return nbRedirection;
            }

            if (i+1 < nbCommandes && !strcmp(tabcmd[i+1], "|")) {
                redirection += 1;
                nbRedirection += 1;
            } else if (redirection) {
                finDeRedirection = 1;
                redirection = 0;
            } else redirection = 0;

            if (nbRedirection) wait(status);

            printf("test\n");

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
    int i, nbArgs, tube[2];

    if (redirection) {
        printf("Redirection !\n");
        if (pipe(tube) == ERR) fatalsyserror(CREATION_PIPE_FAILED);
    }

    // Ces memset evitent une fuite de memoire valgrind
    memset(repertory, '\0', 100);
    memset(nomProgramme, '\0', 100);
    for (i=0; i < 64; i++) {
        args[i] = (char*) calloc(100, sizeof(char));
        memset(args[i], '\0', 100);
    }
    
    if (estCommandeMy(commande)) {
        // Ici nos commandes qu'on a implémenté
        getPwd(repertory);
        strcat(repertory, "/");
        recupererNomProgramme(nomProgramme, commande);
        strcat(repertory, nomProgramme);

        if(fork()==0) {
            if (redirection) {
                if (redirection >= 2) {
                    printf("Lecture ecriture\n");
                    close(STDIN_FILENO);
                    dup(tube[0]);
                    close(STDOUT_FILENO);
                    dup(tube[1]);
                    *status = execlp(repertory, commande, "l", NULL);
                    printf("fail ici redirection >= 2\n");
                } else {
                    close(tube[0]);
                    close(STDOUT_FILENO);
                    dup(tube[1]);
                    close(tube[1]);
                    *status = execlp(repertory, commande, NULL);
                    printf("fail ici redirection else\n");
                }
            } else if (finDeRedirection) {
                printf("Fin de redirection\n");
                close(tube[1]);
                close(STDIN_FILENO);
                dup(tube[0]);
                close(tube[1]);
                *status = execlp(repertory, commande, "l", NULL);
                printf("fail ici fin redirection\n");
            } else *status = execlp(repertory, commande, NULL);
            printf("fail ici else : %s\n", repertory);
            for (i=0; i < 64; i++) free(args[i]);
            fatalsyserror(EXEC_FAIL);
        }
    } else {
        // Ici on peut executer des commandes du shell de base
        // On doit quand même gérer les redirections ici
        recupererNomProgramme(nomProgramme, commande);
        nbArgs = recupererArguments(args, commande);
        args[nbArgs+1] = NULL;
        if(fork()==0) {
            if (redirection) {
                if (redirection >= 2) {
                    printf("Lecture ecriture\n");
                    close(STDIN_FILENO);
                    dup(tube[0]);
                    close(tube[0]);
                    close(STDOUT_FILENO);
                    dup(tube[1]);
                    close(tube[1]);
                    *status = execvp(nomProgramme, args);
                    printf("fail ici\n");
                } else {
                    printf("Ecriture seulement\n");
                    close(tube[0]);
                    close(STDOUT_FILENO);
                    dup(tube[1]);
                    close(tube[1]);
                    *status = execvp(nomProgramme, args);
                    printf("fail ici\n");
                }
            } else if (finDeRedirection) {
                printf("Lecture uniquement\n");
                close(tube[1]);
                close(STDIN_FILENO);
                dup(tube[0]);
                close(tube[0]);
                *status = execvp(nomProgramme, args);
                printf("fail ici\n");
            } else {
                printf("Entre dans else\n");
                *status = execvp(nomProgramme, args);
                printf("fail ici\n");
            }
            for (i=0; i < 64; i++) free(args[i]);
            fatalsyserror(EXEC_FAIL);
        }
    }
    for (i=0; i < 64; i++) free(args[i]);
}

void freeTout(char** commandes) {
    freeCommandes(commandes);
}

int main(void) {
    char** commandes;
    int nbCommandes;
    int status = 0;
    int nbRedirection;

    commandes = allouerMemoireCommandes();
    for(;;) {
        printf("Commence nouvelle boucle\n");
        status = 0;
        commandes = demanderCommande(commandes, &nbCommandes);
        if (!strcmp(*commandes, CMD_EXIT)) {
            freeCommandes(commandes);
            exit(0);
        }
        afficherLesCommandesEntrees(commandes, nbCommandes);
        nbRedirection = executerCommande(commandes, nbCommandes, &status);
        afficherRetour(commandes, nbCommandes, nbRedirection, status);
        printf("Sort retour\n");
        commandes = viderCommande(commandes);
    }
    freeTout(commandes);
    exit(0);
}