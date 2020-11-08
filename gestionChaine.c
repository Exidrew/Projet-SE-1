#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "headers/gestionChaine.h"

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

    *nbCommandes = indexCommandes;
    return commandeSansEspaces;
}

char** demanderCommande(char** commandeSansEspaces, int* nbCommandes) {
    char commandeEntree[sizelgcmd], **commandes;
    putchar('>');
    fgets(commandeEntree, sizelgcmd-1, stdin);

    commandes = retirerEspaces(commandeEntree, commandeSansEspaces, nbCommandes);

    return commandes;
}

void afficherLesCommandesEntrees(char** commandes, int nbCommandes) {
    printf("Voici les commandes entrées : \n");
    for (int i = 0; i <= nbCommandes && commandes[i] != NULL; i++) {
        printf("- %s\n", commandes[i]);
    }
}

void afficherEnBrutLesCommandesEntrees(char** commandes, int nbCommandes) {
    for (int i = 0; i <= nbCommandes && commandes[i] != NULL; i++) {
        printf("%s; ", commandes[i]);
    }
}

void viderCommande(char** commandes, int nbCommande) {
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
        if (cmd[i] == NULL) viderCommande(cmd, i), exit(EXIT_FAILURE);
    }
    
    return cmd;
}

// int main(void) {
//     char** commandes = allouerMemoireCommandes();
//     int nbCommandes;
//     printf("Programme lancé ...\n");
//     nbCommandes = demanderCommande(commandes);

//     afficherLesCommandesEntrees(commandes, nbCommandes);
// }