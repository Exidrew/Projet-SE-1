#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "headers/variablesLocales.h"
#include "headers/liste_chaine.h"

char* retirerAppel(char* commande) {
    while (*commande != ' ') {
        commande++;
    }
    commande++;

    return commande;
}

list_var setVariableLocale(char* commande, list_var variables) {
    char nomVariable[1024], valeurVariable[1024], *cmd;

    printf("Entre setVariable avec la commande : %s\n", commande);
    commande = retirerAppel(commande);
    printf("Après : %s\n", commande);

    // Ajout des noms dans le tableau des noms, puis les valeurs dans tab des valeurs
    cmd = gererVariableLocaleDepuisCommande(commande, nomVariable, '=');
    gererVariableLocaleDepuisCommande(cmd, valeurVariable, ';');

    printf("Nom : %s\n", nomVariable);
    printf("Valeur : %s\n", valeurVariable);
    variables = ajouter(variables, nomVariable, valeurVariable);

    printf("Affichage depuis le fils : \n");
    afficher_variables(variables);

    return variables;
}

list_var delVariableLocale(char* commande, list_var variables) {
    commande = retirerAppel(commande);

    variables = supprimerVar(variables, commande);

    printf("Affichage depuis le fils : \n");
    afficher_variables(variables);
    
    return variables;
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