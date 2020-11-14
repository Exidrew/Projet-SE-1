#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "headers/variables.h"
#include "headers/liste_chaine.h"

char* retirerAppel(char* commande) {
    while (*commande != ' ') {
        commande++;
    }
    commande++;

    return commande;
}

list_var setVariable(char* commande, list_var variables) {
    char nomVariable[1024], valeurVariable[1024], *cmd;

    printf("Entre setVariable avec la commande : %s\n", commande);
    commande = retirerAppel(commande);
    printf("Après : %s\n", commande);

    cmd = gererVariableDepuisCommande2(commande, nomVariable, '=');
    gererVariableDepuisCommande2(cmd, valeurVariable, ';');

    printf("Nom : %s\n", nomVariable);
    printf("Valeur : %s\n", valeurVariable);
    variables = ajouter(variables, nomVariable, valeurVariable);

    printf("Affichage depuis le fils : \n");
    afficher_variables(variables);

    return variables;
}

list_var delVariable(char* commande, list_var variables) {
    commande = retirerAppel(commande);

    variables = supprimerVar(variables, commande);

    printf("Affichage depuis le fils : \n");
    afficher_variables(variables);
    
    return variables;
}

char* gererVariableDepuisCommande2(char* commande, char tab[1024], char fin) {
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