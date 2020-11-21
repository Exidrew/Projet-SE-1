#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>

#include "headers/gestionChaine.h"

#define BLEU(m) "\033[01;34m"m"\033[0m"
#define VERT(m) "\033[01;32m"m"\033[0m"

int doitRetirerEspace(char* commande) {
    return (strncmp(commande, "test", strlen("test")));
}

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
            if (isspace(commande[i]) && doitRetirerEspace(commandeSansEspaces[indexCommandes])) nbEspace++;
        }
        else {
            if (!isspace(commande[i])) {
                commandeSansEspaces[indexCommandes][j] = commande[i];
                j++;
            }
            else if (doitRetirerEspace(commandeSansEspaces[indexCommandes])) nbEspace++;
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
    printf("Entre vider\n");
    for (i = 0; i < sizelgcmd; i++) {
        if (commandes[i] == NULL) commandes[i] = (char*) calloc(sizeWord, sizeof(char));
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

char* chercherNomVariableRemplacer(char* commande, char* nom) {
    int i, j, k = 0;
    for(i=0; commande[i] != '$'; i++);

    // On commence à i+1 pour virer le '$'
    for(j=i+1; commande[j]; j++) {
        nom[k] = commande[j];
        k++;
    }
    nom[k] = '\0';
    return nom;
}

char* remplacerDollarParVariable(char* commande) {
    int i, j, k;
    char* valeur;
    char* nom = (char*) calloc(sizeWord, sizeof(char));
    char* cmd = (char*) calloc(sizeWord, sizeof(char));

    printf("Entre dans le remplacement\n");
    
    nom = chercherNomVariableRemplacer(commande, nom);
    printf("Le nom : %s\n", nom);

    for(i=0; commande[i] != '$'; i++) cmd[i] = commande[i];

    if ((valeur = getenv(nom)) == NULL) {
        return NULL;
    } else {
        for(j=0; valeur[j]; j++) cmd[i++] = valeur[j];
        i = strlen(valeur) - strlen(nom);
    }
    for(k=i; commande[k]; k++) cmd[k] = commande[k];
    cmd[k-1] = '\0';

    printf("La commande apres traitement : %s\n", cmd);
    return cmd;
}

char** remplacerLesVariablesDansLesCommandes(char** commandes, int nbCommandes) {
    int i;
    for (i=0; i < nbCommandes; i++) {
    printf("La commande : %s\n", commandes[i]);
        while (commandes[i] != NULL && strstr(commandes[i], " $")) {
            printf("La commande %s correspond au regex\n", commandes[i]);
            commandes[i] = remplacerDollarParVariable(commandes[i]);
        }
    }
    return commandes;
}

// int main(void) {
//     char** commandes = allouerMemoireCommandes();
//     int nbCommandes;
//     printf("Programme lancé ...\n");
//     commandes = demanderCommande(commandes, &nbCommandes);
//     commandes = remplacerLesVariablesDansLesCommandes(commandes, nbCommandes);
//     printf("Apres gestion :\n");
//     afficherLesCommandesEntrees(commandes, nbCommandes);
// }