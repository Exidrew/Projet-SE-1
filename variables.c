#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "headers/variables.h"


int setVariable(char** tabcmd) {
    char* commande;
    char* nomsVariables[1024], *valeursVariables[1024];
    commande = gestionEspacesCommande(tabcmd);
    printf("La commande finale : %s\n", commande);

    gererVariableDepuisCommande(commande, nomsVariables, '$', '=');
    for (int i = 0; nomsVariables[i] != NULL; i++) {
        printf("Le nom de la %dieme variable : %s\n", i, nomsVariables[i]);
    }

    gererVariableDepuisCommande(commande, valeursVariables, '=', '$');
    for (int i = 0; valeursVariables[i] != NULL; i++) {
        printf("La valeur de la %dieme variable : %s\n", i, valeursVariables[i]);
    }

    return 0;
}

char** gererVariableDepuisCommande(char* commande, char** tab, char debut, char fin) {
    char* nom;
    int vuVariable = 0, nbVariable = 0, longeurNom = 0;
    
    if (debut == '$' && commande[0] != debut) exit(1);
    for (int i = 0; i < strlen(commande); i++) {
        if (commande[i] == debut && !vuVariable){
            nom = (char*) calloc(1024, sizeof(char));
            vuVariable = 1;
        }
        else if (commande[i] == fin && vuVariable) {
            tab[nbVariable] = nom;
            vuVariable = 0;
            nbVariable++;
            longeurNom = 0;
        }
        else if (vuVariable) {
            nom[longeurNom] = commande[i];
            longeurNom++;
        }
    }
    if(fin == '$') tab[nbVariable] = nom;

    return tab;
}

char* gestionEspacesCommande(char** tabcmd) {
    char* commande = (char*) calloc(2048,sizeof(char));
    for (int i = 0; tabcmd[i] != NULL; i++) {
        commande = strcat(commande, tabcmd[i]);
    }
    commande[strlen(commande)] = '\0';
    commande = (char*) realloc(commande, (strlen(commande)+1) * sizeof(char));
    return commande;
}