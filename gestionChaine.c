#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>

#include "headers/gestionChaine.h"

#define BLEU(m) "\033[01;34m"m"\033[0m"
#define VERT(m) "\033[01;32m"m"\033[0m"

char** retirerEspaces(char commande[sizelgcmd], char** commandeSansEspaces, int* nbCommandes) {
    int i, nbEspace = 0, j = 0, indexCommandes = 0;
    for (i = 0; i < strlen(commande); i++) {
        if (commande[i] == '\n') continue;
        if (commande[i] == ';') {
            indexCommandes++;
            nbEspace = 0;
            if (i+1 < strlen(commande) && isspace(commande[i+1]))
                i++;
            j = 0;
            continue;
        }
        if (nbEspace < 1) {
            commandeSansEspaces[indexCommandes][j] = commande[i];
            j++;
            if (isspace(commande[i])) nbEspace++;
        }
        else {
            if (!isspace(commande[i])) {
                commandeSansEspaces[indexCommandes][j] = commande[i];
                j++;
            }
            else nbEspace++;
        }
    }

    *nbCommandes = indexCommandes+1; // +1 car on commence l'index à 0
    return commandeSansEspaces;
}

void affichageLigneShell(){
    char repertory[100];
    char hostName[100];
    getPwd(repertory);
    getComputerName(hostName);
    
    printf(VERT("%s"), hostName);
    putchar(':');
    printf(BLEU("%s"), repertory);
    putchar('$');
    putchar(' ');
}

int getPwd(char *repertory){
    if(getcwd(repertory, UCHAR_MAX) == NULL){
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int getComputerName(char *hostName){
    if(gethostname(hostName, UCHAR_MAX) == -1){
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}   

char** demanderCommande(char** commandeSansEspaces, int* nbCommandes) {
    char commandeEntree[sizelgcmd], **commandes;
    affichageLigneShell();
    fgets(commandeEntree, sizelgcmd-1, stdin);

    commandes = retirerEspaces(commandeEntree, commandeSansEspaces, nbCommandes);

    return commandes;
}

void afficherLesCommandesEntrees(char** commandes, int nbCommandes) {
    printf("Voici les commandes entrées : \n");
    for (int i = 0; i < nbCommandes && commandes[i] != NULL; i++) {
        printf("- %s\n", commandes[i]);
    }
}

void afficherEnBrutLesCommandesEntrees(char** commandes, int nbCommandes) {
    for (int i = 0; i < nbCommandes && commandes[i] != NULL; i++) {
        printf("%s; ", commandes[i]);
    }
}

void viderCommande(char** commandes) {
    int i;
    for (i = 0; i < sizelgcmd; i++) {
        memset(commandes[i], '\0', sizeWord);
    }
}

char** allouerMemoireCommandes() {
    int i;
    char** cmd = (char**) calloc(sizelgcmd, sizeof(char*));
    if (cmd == NULL) free(cmd), exit(EXIT_FAILURE);

    for (i = 0; i < sizelgcmd; i++) {
        cmd[i] = (char*) calloc(sizeWord, sizeof(char));
        if (cmd[i] == NULL) viderCommande(cmd), exit(EXIT_FAILURE);
    }
    
    return cmd;
}

int main(void) {
    char** commandes = allouerMemoireCommandes();
    int nbCommandes;
    printf("Programme lancé ...\n");
    nbCommandes = demanderCommande(commandes, &nbCommandes);
    commandes = remplacerLesVariablesDansLesCommandes(commandes, nbCommandes);
    printf("test");
    afficherLesCommandesEntrees(commandes, nbCommandes);
}