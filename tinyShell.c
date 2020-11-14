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
#include "headers/gestionChaine.h"
#include "headers/variables.h"
#include "headers/tubeCommunication.h"

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

void executerSetVariable(char* tabcmd) {
    listeVariables = setVariable(tabcmd, listeVariables);
}

void executerCommande(char** tabcmd, int nbCommandes) {
    afficherLesCommandesEntrees(tabcmd, nbCommandes);
    for (int i = 0; i <= nbCommandes; i++) {
        if (estCommande(tabcmd[i], CMD_SETVARIABLE)) {
            executerSetVariable(tabcmd[i]);
        }
        else {
            execlp(*tabcmd, *tabcmd, NULL);
            freeCommandes(tabcmd);
            freeVariables(listeVariables);
            syserror(2);
            exit(FAIL_EXEC);
        }
    }
    ecrireVariableVersTube(tubeSetVariable, listeVariables);
    freeCommandes(tabcmd);
    freeVariables(listeVariables);
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
    int nbCommandes;
    pid_t pid;

    commandes = allouerMemoireCommandes();

    for(;;) {
        creerTubeDeCommunication(tubeSetVariable);
        commandes = demanderCommande(commandes, &nbCommandes);
        if (!strcmp(*commandes, CMD_EXIT)) {
            freeCommandes(commandes);
            freeVariables(listeVariables);
            exit(0);
        }
        if ((pid = fork()) == ERR) {
            freeCommandes(commandes);
            fatalsyserror(1);
        }
        if(!pid) executerCommande(commandes, nbCommandes);
        else {
            afficherRetour(commandes, nbCommandes);
            lireVariableDepuisTube(tubeSetVariable);
            fermerTube(tubeSetVariable);
            afficher_variables(listeVariables);
            viderCommande(commandes);
        }
    }
    fermerTube(tubeSetVariable);
    freeCommandes(commandes);
    freeVariables(listeVariables);
    exit(0);
}