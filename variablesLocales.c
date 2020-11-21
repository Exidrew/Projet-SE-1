#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "headers/error.h"
#include "headers/variablesLocales.h"
#include "headers/variables.h"

char* retirerAppel(char* commande) {
    while (*commande != ' ') {
        commande++;
    }
    commande++;

    return commande;
}

int setVariableLocale(char* commande, TableauVariables* variables) {
    char nomVariable[1024], valeurVariable[1024], *cmd;
    printf("Entre setVariable avec la commande : %s\n", commande);
    commande = retirerAppel(commande);
    printf("Après : %s\n", commande);


    cmd = gererVariableLocaleDepuisCommande(commande, nomVariable, '=');
    gererVariableLocaleDepuisCommande(cmd, valeurVariable, ';');
    return setenv(nomVariable, valeurVariable, 1);


    // // Ajout des noms dans le tableau des noms, puis les valeurs dans tab des valeurs
    // cmd = gererVariableLocaleDepuisCommande(commande, nomVariable, '=');
    // gererVariableLocaleDepuisCommande(cmd, valeurVariable, ';');

    // printf("Nom : %s\n", nomVariable);
    // printf("Valeur : %s\n", valeurVariable);
    // ajouterVariable(variables, nomVariable, valeurVariable);
}

int delVariableLocale(char* commande, TableauVariables* variables) {
    commande = retirerAppel(commande);

    supprimerVariable(variables, commande);

    printf("Affichage depuis le fils : \n");
    afficherVariables(variables);
    return 0;
}

char* gererVariableLocaleDepuisCommande(char* commande, char tab[1024], char fin) {
    int i, sizeCommande = strlen(commande);

    for (i = 0; i < sizeCommande; i++) {
        if (commande[0] == fin) break;
        tab[i] = commande[0];
        commande++;
    }
    commande++; // Retrait du '='
    tab[i] = '\0';
    return commande;
}