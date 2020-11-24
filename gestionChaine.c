#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <regex.h>

#include "headers/error.h"
#include "headers/gestionChaine.h"

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

char** demanderCommande(char** commande, int* nbCommandes) {
    char commandeEntree[sizelgcmd], **commandes;
    affichageLigneShell();
    fgets(commandeEntree, sizelgcmd-1, stdin);

    commandes = retirerEspaces(commandeEntree, commande, nbCommandes);

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

char* chercherNomVariableRemplacer(char* commande, char* nom, int* index) {
    int i=0, j, k = 0;

    while (commande[i] != '$') {
        if (i < strlen(commande) && !isspace(commande[i]) && commande[i+1] == '$') i++;
        i++;
    }

    *index = i;
    // On commence à m+1 pour virer le " $"
    for(j=i+1; commande[j]; j++) {
        nom[k] = commande[j];
        k++;
    }
    nom[k] = '\0';
    return nom;
}

void remplacerDollarParVariable(char* commande, char* cmd) {
    int j, k, m, index;
    char* valeur;
    char* nom = (char*) calloc(sizeWord, sizeof(char));

    
    nom = chercherNomVariableRemplacer(commande, nom, &index);

    for(m=0; commande[m] && m < index; m++) {
        cmd[m] = commande[m];
    }

    if ((valeur = getenv(nom)) == NULL) {
        cmd = NULL;
    } else {
        for(j=0; valeur[j]; j++) {
            cmd[m++] = valeur[j];
        }
    }

    for(k=m; commande[k+strlen(nom)]; k++) cmd[k] = commande[k+strlen(nom)];
    cmd[k] = '\0';

    free(nom);
}

char** remplacerLesVariablesDansLesCommandes(char** commandes, int nbCommandes, int* status) {
    int i,j;
    regex_t regex;
    char* cmd = (char*) calloc(sizeWord, sizeof(char));
    const char schema[19] = " +\\$[a-zA-Z0-9_-]+";
    if (regcomp(&regex, schema, REG_EXTENDED)) {
        regfree(&regex);
        fatalsyserror(8);
    }
    for (i=0; i < nbCommandes; i++) {
        while (commandes[i] != NULL && regexec(&regex, commandes[i], 0, NULL, 0) == 0) {
            remplacerDollarParVariable(commandes[i], cmd);
            if (cmd != NULL) {
                memset(commandes[i], '\0', sizeWord);
                for (j=0; cmd[j]; j++) {
                    commandes[i][j] = cmd[j];
                }
            }
            else {
                free(cmd);
                *status = -1;
                return commandes;
            }
        }
    }
    if (cmd != NULL) free(cmd);
    regfree(&regex);
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